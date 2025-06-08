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
#include "open_loop.h"
#include "ramp_ext_mngr.h"
#include "pwm_curr_fdbk.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
OpenLoop_Handle_t *pOpenLoop[1] = {MC_NULL};   

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

void FOC_Clear(uint8_t bMotor)
{
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

 void FOC_Init()
{
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

    //Setting the control mode
    // MCI_SetSpeedMode(&Mci[M1]);
    FOCVars[M1].bDriveInput = INTERNAL;
    STC_SetControlMode(Mci[M1].pSTC, MCM_SPEED_MODE); //TODO
    Mci[M1].LastModalitySetByUser = MCM_SPEED_MODE; //MCM_OPEN_LOOP_VOLTAGE_MODE

    // MCI_ExecTorqueRamp(&Mci[M1], STC_GetDefaultIqdref(pSTC[M1]).q, 0);
}
