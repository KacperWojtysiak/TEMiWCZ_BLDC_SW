/**
  ******************************************************************************
  * @file           : MC.c
  * @author         : 
  * @brief          : Code for all motor control related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "MC.h"
#include "mc_type.h"
#include "mc_config.h"

#include "FOC.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/
void mc_lock_pins (void)
{
    LL_GPIO_LockPin(SENS_I_A_GPIO_Port, SENS_I_A_Pin);
    LL_GPIO_LockPin(SENS_I_B_GPIO_Port, SENS_I_B_Pin);
    LL_GPIO_LockPin(SENS_I_C_GPIO_Port, SENS_I_C_Pin);
    LL_GPIO_LockPin(M1_PWM_UH_GPIO_Port, M1_PWM_UH_Pin);
    LL_GPIO_LockPin(M1_PWM_VH_GPIO_Port, M1_PWM_VH_Pin);
    LL_GPIO_LockPin(M1_PWM_VL_GPIO_Port, M1_PWM_VL_Pin);
    LL_GPIO_LockPin(M1_PWM_WH_GPIO_Port, M1_PWM_WH_Pin);
    LL_GPIO_LockPin(M1_PWM_WL_GPIO_Port, M1_PWM_WL_Pin);
    LL_GPIO_LockPin(M1_PWM_UL_GPIO_Port, M1_PWM_UL_Pin);
}

void MCboot( MCI_Handle_t* pMC)
{
  if (NULL == pMC)
  {
    return;
  }
  else
  {
    bMCBootCompleted = 0;
    FOC_Init();
    PID_HandleInit(&PIDSpeedHandle_M1);
    VSS_Init(&VirtualSpeedSensorM1);
    VVBS_Init(&BusVoltageSensor_M1);

    bMCBootCompleted = 1U;
  }
}

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void MC_Init(void){
  // Reconfigure the SysTick interrupt to fire every 500 us.
  (void)HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / SYS_TICK_FREQUENCY);
  HAL_NVIC_SetPriority(SysTick_IRQn, uwTickPrio, 0U);

/* Initialize the Motor Control Subsystem */
  MCboot(&Mci[M1]);
  mc_lock_pins();
}

void MCI_FaultProcessing(MCI_Handle_t *pHandle, uint16_t hSetErrors, uint16_t hResetErrors){
    pHandle->CurrentFaults = (pHandle->CurrentFaults | hSetErrors ) & (~hResetErrors);
    pHandle->PastFaults |= hSetErrors;
}

int16_t MCI_GetImposedMotorDirection(MCI_Handle_t *pHandle) 
{
  int16_t retVal = 1;

#ifdef NULL_PTR_CHECK_MC_INT
  if (MC_NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    switch (pHandle->lastCommand)
    {
      case MCI_CMD_EXECSPEEDRAMP:
      {
        if (pHandle->hFinalSpeed < 0)
        {
          retVal = -1;
        }
        else
        {
          /* Nothing to do */
        }
        break;
      }

      case MCI_CMD_EXECTORQUERAMP:
      {
        if (pHandle->hFinalTorque < 0)
        {
          retVal = -1;
        }
        else
        {
          /* Nothing to do */
        }
        break;
      }

      case MCI_CMD_SETCURRENTREFERENCES:
      {
        if (pHandle->Iqdref.q < 0)
        {
          retVal = -1;
        }
        else
        {
          /* Nothing to do */
        }
        break;
       }
      default:
        break;
    }
#ifdef NULL_PTR_CHECK_MC_INT
  }
#endif
  return (retVal);
}

void MCI_ExecBufferedCommands(MCI_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_MC_INT
  if (NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    if ( pHandle->CommandState == MCI_COMMAND_NOT_ALREADY_EXECUTED )
    {
      bool commandHasBeenExecuted = false;
      switch (pHandle->lastCommand)
      {
        case MCI_CMD_EXECSPEEDRAMP:
        {
          pHandle->pFOCVars->bDriveInput = INTERNAL;
          STC_SetControlMode(pHandle->pSTC, MCM_SPEED_MODE);
          VSS_SetMecAcceleration( pHandle->pVSS, pHandle->hFinalSpeed, pHandle->hDurationms);
          commandHasBeenExecuted = STC_ExecRamp(pHandle->pSTC, pHandle->hFinalSpeed, pHandle->hDurationms);
          break;
        }

        case MCI_CMD_EXECTORQUERAMP:
        {
          pHandle->pFOCVars->bDriveInput = INTERNAL;
          STC_SetControlMode(pHandle->pSTC, MCM_TORQUE_MODE);
          commandHasBeenExecuted = STC_ExecRamp(pHandle->pSTC, pHandle->hFinalTorque, pHandle->hDurationms);
          break;
        }

        case MCI_CMD_SETCURRENTREFERENCES:
        {
          pHandle->pFOCVars->bDriveInput = EXTERNAL;
          pHandle->pFOCVars->Iqdref = pHandle->Iqdref;
          commandHasBeenExecuted = true;
          break;
        }

        case MCI_CMD_SETOPENLOOPCURRENT:
        {
          pHandle->pFOCVars->bDriveInput = EXTERNAL;
          VSS_SetMecAcceleration( pHandle->pVSS, pHandle->hFinalSpeed, pHandle->hDurationms);
          commandHasBeenExecuted = true;
          break;
        }

        case MCI_CMD_SETOPENLOOPVOLTAGE:
        {
          pHandle->pFOCVars->bDriveInput = EXTERNAL;
          VSS_SetMecAcceleration( pHandle->pVSS, pHandle->hFinalSpeed, pHandle->hDurationms);
          commandHasBeenExecuted = true;
          break;
        }

        default:
          break;
      }

      if (commandHasBeenExecuted)
      {
        pHandle->CommandState = MCI_COMMAND_EXECUTED_SUCCESSFULLY;
      }
      else
      {
        pHandle->CommandState = MCI_COMMAND_EXECUTED_UNSUCCESSFULLY;
      }
    }
#ifdef NULL_PTR_CHECK_MC_INT
  }
#endif
}

bool MCI_StartMotor()
{
  bool retVal = false;
if ((IDLE ==  Mci->State) &&
    (MC_NO_FAULTS ==  Mci->PastFaults) &&
    (MC_NO_FAULTS ==  Mci->CurrentFaults))
{
    Mci->DirectCommand = MCI_START;
    Mci->CommandState = MCI_COMMAND_NOT_ALREADY_EXECUTED;
    retVal = true;
}
  return (retVal);
}

bool MCI_StopMotor()
{
    bool retVal = false;
    bool status;
    MCI_State_t State;

    State = Mci->State;
    if ((IDLE == State) || (ICLWAIT == State))
    {
        status = false;
    }
    else
    {
        status = true;
    }

    if  ((MC_NO_FAULTS ==  Mci->PastFaults) &&
        (MC_NO_FAULTS ==  Mci->CurrentFaults) &&
        (status == true))
    {
        Mci->DirectCommand = MCI_STOP;
        retVal = true;
    }
  return (retVal);
}

void MCI_ExecSpeedRamp(int16_t hFinalSpeed, uint16_t hDurationms)
{
    Mci->lastCommand = MCI_CMD_EXECSPEEDRAMP;
    Mci->hFinalSpeed = hFinalSpeed;
    Mci->hDurationms = hDurationms;
    Mci->CommandState = MCI_COMMAND_NOT_ALREADY_EXECUTED;
}

uint32_t MCI_GetFaultState(MCI_Handle_t *pHandle){
    uint32_t LocalFaultState;
    LocalFaultState = (uint32_t)(pHandle->PastFaults);
    LocalFaultState |= (uint32_t)(pHandle->CurrentFaults) << 16;
    return (LocalFaultState);
}