/**
  ******************************************************************************
  * @file           : Scheduler.c
  * @author         : 
  * @brief          : Code for all Scheduler related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "Scheduler.h"

#include "mc_config.h"
#include "mc_config_common.h"
#include "pwm_common.h"
#include "sto_pll_speed_pos_fdbk.h"
#include "speed_torq_ctrl.h"
#include "revup_ctrl.h"
#include "pid_regulator.h"
#include "open_loop.h"
#include "ramp_ext_mngr.h"
#include "pwm_curr_fdbk.h"
#include "pqd_motor_power_measurement.h"

#include "MC.h"
#include "FOC.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
volatile uint16_t tick = 1;
static uint16_t mcTick = 0;

static volatile uint16_t hBootCapDelayCounterM1 = 0;
static volatile uint16_t hStopPermanencyCounterM1 = 0;

#define M1_CHARGE_BOOT_CAP_TICKS          (((uint16_t)SYS_TICK_FREQUENCY * (uint16_t)10) / 1000U)
#define M1_CHARGE_BOOT_CAP_DUTY_CYCLES ((uint32_t)0.000\
                                      * ((uint32_t)PWM_PERIOD_CYCLES / 2U))
#define M2_CHARGE_BOOT_CAP_TICKS         (((uint16_t)SYS_TICK_FREQUENCY * (uint16_t)10) / 1000U)
#define M2_CHARGE_BOOT_CAP_DUTY_CYCLES ((uint32_t)0\
                                      * ((uint32_t)PWM_PERIOD_CYCLES2 / 2U))

#define STOPPERMANENCY_MS              ((uint16_t)400)
#define STOPPERMANENCY_MS2             ((uint16_t)400)
#define STOPPERMANENCY_TICKS           (uint16_t)((SYS_TICK_FREQUENCY * STOPPERMANENCY_MS)  / ((uint16_t)1000))
#define STOPPERMANENCY_TICKS2          (uint16_t)((SYS_TICK_FREQUENCY * STOPPERMANENCY_MS2) / ((uint16_t)1000))

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/
// HAL_SYSTICK_Callback()
void TSK_MediumFrequencyTaskM1();
void TSK_SafetyTask();
void TSK_SafetyTask_PWMOFF(uint8_t bMotor);
/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void Task1ms(){

}

void Task10ms(){
    // HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
}

void Task100ms(){
    HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
    // CDC_Transmit_FS((uint8_t*)"DUPA", 4);
}

void Task(){
    Task1ms();
    if ( (tick % 10) == 0){
        Task10ms();
    }
    if ( (tick % 100) == 0){
        Task100ms();
    }
    tick ++;
}

void MC_RunMotorControlTasks(void)
{
  if ( bMCBootCompleted){
    /* ** Medium Frequency Tasks ** */
    if(mcTick > 0u){
      mcTick--;
    }
    else{
      TSK_MediumFrequencyTaskM1();
      mcTick = (uint16_t)MF_TASK_OCCURENCE_TICKS;
    }
    if(hBootCapDelayCounterM1 > 0U)
    {
      hBootCapDelayCounterM1--;
    }
    if(hStopPermanencyCounterM1 > 0U)
    {
      hStopPermanencyCounterM1--;
    }
    /* Safety task is run after Medium Frequency task so that
     * it can overcome actions they initiated if needed */
    TSK_SafetyTask(); 
  }
}

void TSK_SetChargeBootCapDelayM1(uint16_t hTickCount)
{
    hBootCapDelayCounterM1 = hTickCount;
}

void TSK_MF_StopProcessing(uint8_t motor){
    R3_1_SwitchOffPWM(pwmcHandle[motor]);

    FOC_Clear(motor);

    hStopPermanencyCounterM1 = STOPPERMANENCY_TICKS;
    // TSK_SetStopPermanencyTimeM1(STOPPERMANENCY_TICKS); //TODO
    Mci[motor].State = STOP;
}

bool TSK_ChargeBootCapDelayHasElapsedM1(void)
{
  bool retVal = false;
  if (((uint16_t)0) == hBootCapDelayCounterM1)
  {
    retVal = true;
  }
  return (retVal);
}

bool TSK_StopPermanencyTimeHasElapsedM1(void)
{
  bool retVal = false;
  if (((uint16_t)0) == hStopPermanencyCounterM1)
  {
    retVal = true;
  }
  return (retVal);
}

void TSK_MediumFrequencyTaskM1(void){
  int16_t wAux = 0;
  MC_ControlMode_t mode;

  mode = Mci[M1].LastModalitySetByUser;
  bool IsSpeedReliable = STO_PLL_CalcAvrgMecSpeedUnit(&STO_PLL_M1, &wAux);
  PQD_CalcElMotorPower(pMPM[M1]);

  if (Mci[M1].CurrentFaults == MC_NO_FAULTS)
  {
    if (Mci[M1].PastFaults == MC_NO_FAULTS)
    {
      switch (Mci[M1].State)
      {
        case IDLE:
        {
          if ((MCI_START == Mci[M1].DirectCommand) || (MCI_MEASURE_OFFSETS == Mci[M1].DirectCommand))
          {
            if ( mode != MCM_OPEN_LOOP_VOLTAGE_MODE && mode != MCM_OPEN_LOOP_CURRENT_MODE)
            {
              RUC_Clear(&RevUpControlM1, MCI_GetImposedMotorDirection(&Mci[M1]));
            }
            if (pwmcHandle[M1]->offsetCalibStatus == false)
            {
              (void)PWMC_CurrentReadingCalibr(pwmcHandle[M1], CRC_START);
              Mci[M1].State = OFFSET_CALIB;
            }
            else
            {
                /* Calibration already done. Enables only TIM channels */
                pwmcHandle[M1]->OffCalibrWaitTimeCounter = 1u;
                (void)PWMC_CurrentReadingCalibr(pwmcHandle[M1], CRC_EXEC);
                R3_1_TurnOnLowSides(pwmcHandle[M1],M1_CHARGE_BOOT_CAP_DUTY_CYCLES);
                //hBootCapDelayCounterM1 = M1_CHARGE_BOOT_CAP_TICKS; //TODO
                TSK_SetChargeBootCapDelayM1(M1_CHARGE_BOOT_CAP_TICKS);
                Mci[M1].State = CHARGE_BOOT_CAP;
            }
          }
          break;
        }

        case OFFSET_CALIB:
        {
          if (MCI_STOP == Mci[M1].DirectCommand)
          {
            TSK_MF_StopProcessing(M1);
          }
          else
          {
            if (PWMC_CurrentReadingCalibr(pwmcHandle[M1], CRC_EXEC))
            {
              if (MCI_MEASURE_OFFSETS == Mci[M1].DirectCommand)
              {
                FOC_Clear(M1);
                Mci[M1].DirectCommand = MCI_NO_COMMAND;
                Mci[M1].State = IDLE;
              }
              else
              {
                R3_1_TurnOnLowSides(pwmcHandle[M1],M1_CHARGE_BOOT_CAP_DUTY_CYCLES);
                TSK_SetChargeBootCapDelayM1(M1_CHARGE_BOOT_CAP_TICKS);
                Mci[M1].State = CHARGE_BOOT_CAP;
              }
            }
          }
          break;
        }

        case CHARGE_BOOT_CAP:
        {
          if (MCI_STOP == Mci[M1].DirectCommand)
          {
            TSK_MF_StopProcessing(M1);
          }
          else
          {
            if (TSK_ChargeBootCapDelayHasElapsedM1())
            {
              R3_1_SwitchOffPWM(pwmcHandle[M1]);
              FOCVars[M1].bDriveInput = EXTERNAL;
              STC_SetSpeedSensor( pSTC[M1], &VirtualSpeedSensorM1._Super );

              STO_PLL_Clear(&STO_PLL_M1);

              FOC_Clear( M1 );

              if (MCM_OPEN_LOOP_VOLTAGE_MODE == mode || MCM_OPEN_LOOP_CURRENT_MODE == mode)
              {
                Mci[M1].State = RUN;
              }
              else
              {
                Mci[M1].State = START;
              }
              PWMC_SwitchOnPWM(pwmcHandle[M1]);
            }
          }
          break;
        }

        case START:
        {
          if (MCI_STOP == Mci[M1].DirectCommand)
          {
            TSK_MF_StopProcessing(M1);
          }
          else
          {
            /* Mechanical speed as imposed by the Virtual Speed Sensor during the Rev Up phase. */
            int16_t hForcedMecSpeedUnit;
            qd_t IqdRef;
            bool ObserverConverged;

            /* Execute the Rev Up procedure */
            if(! RUC_Exec(&RevUpControlM1))
            {
            /* The time allowed for the startup sequence has expired */
              MCI_FaultProcessing(&Mci[M1], MC_START_UP, 0);
            }
            else
            {
              /* Execute the torque open loop current start-up ramp:
               * Compute the Iq reference current as configured in the Rev Up sequence */
              IqdRef.q = STC_CalcTorqueReference(pSTC[M1]);
              IqdRef.d = FOCVars[M1].UserIdref;
              /* Iqd reference current used by the High Frequency Loop to generate the PWM output */
              FOCVars[M1].Iqdref = IqdRef;
           }

            (void)VSS_CalcAvrgMecSpeedUnit(&VirtualSpeedSensorM1, &hForcedMecSpeedUnit);

            /* Check that startup stage where the observer has to be used has been reached */
            if (true == RUC_FirstAccelerationStageReached(&RevUpControlM1))
            {
              ObserverConverged = STO_PLL_IsObserverConverged(&STO_PLL_M1, &hForcedMecSpeedUnit);
              STO_SetDirection(&STO_PLL_M1, (int8_t)MCI_GetImposedMotorDirection(&Mci[M1]));

              (void)VSS_SetStartTransition(&VirtualSpeedSensorM1, ObserverConverged);
            }
            else
            {
              ObserverConverged = false;
            }
            if (ObserverConverged)
            {
              qd_t StatorCurrent = MCM_Park(FOCVars[M1].Ialphabeta, SPD_GetElAngle(&STO_PLL_M1._Super));

              /* Start switch over ramp. This ramp will transition from the revup to the closed loop FOC */
              REMNG_Init(pREMNG[M1]);
              (void)REMNG_ExecRamp(pREMNG[M1], FOCVars[M1].Iqdref.q, 0);
              (void)REMNG_ExecRamp(pREMNG[M1], StatorCurrent.q, TRANSITION_DURATION);

              Mci[M1].State = SWITCH_OVER;
            }
          }
          break;
        }

        case SWITCH_OVER:
        {
          if (MCI_STOP == Mci[M1].DirectCommand)
          {
            TSK_MF_StopProcessing(M1);
          }
          else
          {
            bool LoopClosed;
            int16_t hForcedMecSpeedUnit;

            if (! RUC_Exec(&RevUpControlM1))
            {
              /* The time allowed for the startup sequence has expired */
              MCI_FaultProcessing(&Mci[M1], MC_START_UP, 0);
            }
            else
            {
              /* Compute the virtual speed and positions of the rotor.
                 The function returns true if the virtual speed is in the reliability range */
              LoopClosed = VSS_CalcAvrgMecSpeedUnit(&VirtualSpeedSensorM1, &hForcedMecSpeedUnit);
              /* Check if the transition ramp has completed. */
              bool tempBool;
              tempBool = VSS_TransitionEnded(&VirtualSpeedSensorM1);
              LoopClosed = LoopClosed || tempBool;

              /* If any of the above conditions is true, the loop is considered closed.
                 The state machine transitions to the RUN state */
              if (true ==  LoopClosed)
              {
#if PID_SPEED_INTEGRAL_INIT_DIV == 0
                PID_SetIntegralTerm(&PIDSpeedHandle_M1, 0);
#else
                PID_SetIntegralTerm(&PIDSpeedHandle_M1,
                                    (((int32_t)FOCVars[M1].Iqdref.q * (int16_t)PID_GetKIDivisor(&PIDSpeedHandle_M1))
                                    / PID_SPEED_INTEGRAL_INIT_DIV));
#endif
                STC_SetSpeedSensor(pSTC[M1], &STO_PLL_M1._Super); /* Observer has converged */
                // FOC_InitAdditionalMethods(M1);
                FOC_CalcCurrRef(M1);
                STC_ForceSpeedReferenceToCurrentSpeed(pSTC[M1]); /* Init the reference speed to current speed */
                MCI_ExecBufferedCommands(&Mci[M1]); /* Exec the speed ramp after changing of the speed sensor */
                Mci[M1].State = RUN;
              }
            }
          }
          break;
        }

        case RUN:
        {
          if (MCI_STOP == Mci[M1].DirectCommand)
          {
            TSK_MF_StopProcessing(M1);
          }
          else
          {
            MCI_ExecBufferedCommands(&Mci[M1]);
            if (mode != MCM_OPEN_LOOP_VOLTAGE_MODE && mode != MCM_OPEN_LOOP_CURRENT_MODE)
            {
              FOC_CalcCurrRef(M1);
              if(!IsSpeedReliable)
              {
                MCI_FaultProcessing(&Mci[M1], MC_SPEED_FDBK, 0);
              }
            }
            else
            {
              int16_t hForcedMecSpeedUnit;
              /* Open Loop */
              VSS_CalcAvrgMecSpeedUnit( &VirtualSpeedSensorM1, &hForcedMecSpeedUnit);
              OL_Calc(pOpenLoop[M1]);
            }
          }
          break;
        }

        case STOP:
        {
          if (TSK_StopPermanencyTimeHasElapsedM1())
          {
            STC_SetSpeedSensor(pSTC[M1], &VirtualSpeedSensorM1._Super);    /* Sensor-less */
            VSS_Clear(&VirtualSpeedSensorM1); /* Reset measured speed in IDLE */
            Mci[M1].DirectCommand = MCI_NO_COMMAND;
            Mci[M1].State = IDLE;
          }
          break;
        }

        case FAULT_OVER:
        {
          if (MCI_ACK_FAULTS == Mci[M1].DirectCommand)
          {
            Mci[M1].DirectCommand = MCI_NO_COMMAND;
            Mci[M1].State = IDLE;
          }
          break;
        }

        case FAULT_NOW:
        {
          Mci[M1].State = FAULT_OVER;
          break;
        }

        default:
          break;
       }
    }
    else
    {
      Mci[M1].State = FAULT_OVER;
    }
  }
  else
  {
    Mci[M1].State = FAULT_NOW;
  }
}

void TSK_SafetyTask(void)
{
  if (1U == bMCBootCompleted)
  {
    TSK_SafetyTask_PWMOFF(M1);
    /* User conversion execution */
    // RCM_ExecUserConv(); //TODO Not important
  }
}

 void TSK_SafetyTask_PWMOFF(uint8_t bMotor)
{
  uint16_t CodeReturn = MC_NO_ERROR;
  uint8_t lbmotor = M1;
  /* Check for fault if FW protection is activated. It returns MC_OVER_TEMP or MC_NO_ERROR */

/* Due to warning array subscript 1 is above array bounds of PWMC_Handle_t *[1] [-Warray-bounds] */
   CodeReturn |= PWMC_IsFaultOccurred(pwmcHandle[lbmotor]);     /* check for fault. It return MC_OVER_CURR or MC_NO_FAULTS
                                                     (for STM32F30x can return MC_OVER_VOLT in case of HW Overvoltage) */

  MCI_FaultProcessing(&Mci[bMotor], CodeReturn, ~CodeReturn); /* Process faults */

  if (MCI_GetFaultState(&Mci[bMotor]) != (uint32_t)MC_NO_FAULTS)
  {
    PWMC_SwitchOffPWM(pwmcHandle[bMotor]);
    FOC_Clear(bMotor);
  }
}

