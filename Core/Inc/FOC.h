/**
  ******************************************************************************
  * @file           : FOC.h
  * @author         : 
  * @brief          : Header for all FOC related functionalities.
  ******************************************************************************
  */
#ifndef __FOC_H
#define __FOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
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

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void FOC_Init();
void FOC_Clear(uint8_t bMotor);


#ifdef __cplusplus
}
#endif

#endif /* __FOC_H */
