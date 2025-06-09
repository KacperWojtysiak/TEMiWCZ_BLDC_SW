/**
  ******************************************************************************
  * @file           : FOC.c
  * @author         : 
  * @brief          : Header for all FOC related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "FOC.h"

#include "mc_config.h"
#include "mc_config_common.h"
#include "pwm_common.h"
#include "sto_pll_speed_pos_fdbk.h"
#include "speed_torq_ctrl.h"
#include "revup_ctrl.h"
#include "pid_regulator.h"
#include "ramp_ext_mngr.h"
#include "pwm_curr_fdbk.h"
#include "mc_math.h"
#include "circle_limitation.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
OpenLoop_Handle_t *pOpenLoop[1] = {MC_NULL};

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/
static uint16_t FOC_CurrControllerM1();

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

void FOC_Clear(uint8_t bMotor){
  MC_ControlMode_t mode;

  mode = Mci[bMotor].LastModalitySetByUser;

  ab_t NULL_ab = {((int16_t)0), ((int16_t)0)};
  qd_t NULL_qd = {((int16_t)0), ((int16_t)0)};
  alphabeta_t NULL_alphabeta = {((int16_t)0), ((int16_t)0)};

  FOCVars[bMotor].Iab = NULL_ab;
  FOCVars[bMotor].Ialphabeta = NULL_alphabeta;
  FOCVars[bMotor].Iqd = NULL_qd;
  if ( mode != MCM_OPEN_LOOP_VOLTAGE_MODE && mode != MCM_OPEN_LOOP_CURRENT_MODE)
  {
    FOCVars[bMotor].Iqdref = NULL_qd;
  }
  else
  {
    /* Nothing to do */
  }
  FOCVars[bMotor].hTeref = (int16_t)0;
  FOCVars[bMotor].Vqd = NULL_qd;
  FOCVars[bMotor].Valphabeta = NULL_alphabeta;
  FOCVars[bMotor].hElAngle = (int16_t)0;

  PID_SetIntegralTerm(pPIDIq[bMotor], ((int32_t)0));
  PID_SetIntegralTerm(pPIDId[bMotor], ((int32_t)0));

  STC_Clear(pSTC[bMotor]);

  PWMC_SwitchOffPWM(pwmcHandle[bMotor]);
}

void FOC_Init(){
  pwmcHandle[M1] = &PWM_Handle_M1._Super;
  R3_1_Init(&PWM_Handle_M1);
  startTimers();
  PID_HandleInit(&PIDSpeedHandle_M1);
  STO_PLL_Init (&STO_PLL_M1);
  STC_Init(pSTC[M1],&PIDSpeedHandle_M1, &STO_PLL_M1._Super);
  RUC_Init(&RevUpControlM1, pSTC[M1], &VirtualSpeedSensorM1, &STO_M1, pwmcHandle[M1]);
  PID_HandleInit(&PIDIqHandle_M1);
  PID_HandleInit(&PIDIdHandle_M1);
  pMPM[M1]->pVBS = &(BusVoltageSensor_M1._Super);
  pMPM[M1]->pFOCVars = &FOCVars[M1];

  OL_Init(&OpenLoop_ParamsM1, &VirtualSpeedSensorM1);
  pOpenLoop[M1] = &OpenLoop_ParamsM1;

  pREMNG[M1] = &RampExtMngrHFParamsM1;
  REMNG_Init(pREMNG[M1]);

  FOC_Clear(M1);
  FOCVars[M1].bDriveInput = EXTERNAL;
  FOCVars[M1].Iqdref = STC_GetDefaultIqdref(pSTC[M1]);
  FOCVars[M1].UserIdref = STC_GetDefaultIqdref(pSTC[M1]).d;
  MCI_SetSpeedMode(&Mci[M1]);

  MCI_ExecTorqueRamp(&Mci[M1], STC_GetDefaultIqdref(pSTC[M1]).q, 0);
}

void FOC_HighFrequencyTask(){
  uint16_t hFOCreturn;
  Observer_Inputs_t STO_Inputs;

  STO_Inputs.Valfa_beta = FOCVars[M1].Valphabeta;
  if (SWITCH_OVER == Mci[M1].State)
  {
    if (!REMNG_RampCompleted(pREMNG[M1]))
    {
      FOCVars[M1].Iqdref.q = (int16_t)REMNG_Calc(pREMNG[M1]);
    }
  }
  hFOCreturn = FOC_CurrControllerM1();
  if(hFOCreturn == MC_DURATION)
  {
    MCI_FaultProcessing(&Mci[M1], MC_DURATION, 0); //TODO
  }
  else
  {
    bool IsAccelerationStageReached = RUC_FirstAccelerationStageReached(&RevUpControlM1);
    STO_Inputs.Ialfa_beta = FOCVars[M1].Ialphabeta;  
    STO_Inputs.Vbus = VBS_GetAvBusVoltage_d(&(BusVoltageSensor_M1._Super));
    (void)STO_PLL_CalcElAngle(&STO_PLL_M1, &STO_Inputs);
    STO_PLL_CalcAvrgElSpeedDpp(&STO_PLL_M1);
    if (false == IsAccelerationStageReached)
    {
      STO_ResetPLL(&STO_PLL_M1);
    }
    /* Only for sensor-less or open loop */
    if((START == Mci[M1].State) || (SWITCH_OVER == Mci[M1].State) || (RUN == Mci[M1].State))
    {
      int16_t hObsAngle = SPD_GetElAngle(&STO_PLL_M1._Super);
      (void)VSS_CalcElAngle(&VirtualSpeedSensorM1, &hObsAngle);
    }
  }
}

inline uint16_t FOC_CurrControllerM1(void)
{
  qd_t Iqd, Vqd;
  ab_t Iab;
  alphabeta_t Ialphabeta, Valphabeta;
  int16_t hElAngle;
  uint16_t hCodeError;
  SpeednPosFdbk_Handle_t *speedHandle;
  MC_ControlMode_t mode;

  mode = MCI_GetControlMode( &Mci[M1] );
  speedHandle = STC_GetSpeedSensor(pSTC[M1]);
  hElAngle = SPD_GetElAngle(speedHandle);
  hElAngle += SPD_GetInstElSpeedDpp(speedHandle)*PARK_ANGLE_COMPENSATION_FACTOR;
  PWMC_GetPhaseCurrents(pwmcHandle[M1], &Iab);
  // RCM_ReadOngoingConv();
  // RCM_ExecNextConv();
  Ialphabeta = MCM_Clarke(Iab);
  Iqd = MCM_Park(Ialphabeta, hElAngle);
  Vqd.q = PI_Controller(pPIDIq[M1], (int32_t)(FOCVars[M1].Iqdref.q) - Iqd.q);
  Vqd.d = PI_Controller(pPIDId[M1], (int32_t)(FOCVars[M1].Iqdref.d) - Iqd.d);
  if (mode == MCM_OPEN_LOOP_VOLTAGE_MODE)
  {
    Vqd = OL_VqdConditioning(pOpenLoop[M1]);
  }
  else
  {
    /* Nothing to do */
  }
  Vqd = Circle_Limitation(&CircleLimitationM1, Vqd);
  hElAngle += SPD_GetInstElSpeedDpp(speedHandle)*REV_PARK_ANGLE_COMPENSATION_FACTOR;
  Valphabeta = MCM_Rev_Park(Vqd, hElAngle);
  hCodeError = PWMC_SetPhaseVoltage(pwmcHandle[M1], Valphabeta);

  FOCVars[M1].Vqd = Vqd;
  FOCVars[M1].Iab = Iab;
  FOCVars[M1].Ialphabeta = Ialphabeta;
  FOCVars[M1].Iqd = Iqd;
  FOCVars[M1].Valphabeta = Valphabeta;
  FOCVars[M1].hElAngle = hElAngle;

  return (hCodeError);
}

void FOC_CalcCurrRef(uint8_t bMotor)
{
  qd_t IqdTmp;

  /* Enter critical section */
  /* Disable interrupts to avoid any interruption during Iqd reference latching */
  /* to avoid MF task writing them while HF task reading them */
  __disable_irq();
  IqdTmp = FOCVars[bMotor].Iqdref;

  /* Exit critical section */
  __enable_irq();

  MC_ControlMode_t mode;

  mode = Mci[bMotor].LastModalitySetByUser;
  if (INTERNAL == FOCVars[bMotor].bDriveInput
               && (mode != MCM_OPEN_LOOP_VOLTAGE_MODE && mode != MCM_OPEN_LOOP_CURRENT_MODE))
  {
    FOCVars[bMotor].hTeref = STC_CalcTorqueReference(pSTC[bMotor]);
    IqdTmp.q = FOCVars[bMotor].hTeref;

  }

  /* Enter critical section */
  /* Disable interrupts to avoid any interruption during Iqd reference restoring */
  __disable_irq();
  FOCVars[bMotor].Iqdref = IqdTmp;

  /* Exit critical section */
  __enable_irq();
}