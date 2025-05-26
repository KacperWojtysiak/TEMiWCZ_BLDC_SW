/**
  ******************************************************************************
  * @file    speed_pos_fdbk_hso.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file contains all definitions and functions prototypes for the
  *          sensorless HSO component of the Motor Control SDK.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  * @ingroup speed_pos_fdbk_hso
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPEEDNPOSFDBK_HSO_H_
#define _SPEEDNPOSFDBK_HSO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "mc_math.h"
#include "flash_parameters.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup SpeednPosFdbk
  * @{
  */

/** @addtogroup speed_pos_fdbk_hso
  * @{
  */

/* Exported defines ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  Not applicable
  */
typedef enum _FOC_Rs_update_Select_e_
{
    FOC_Rs_update_None,   /*!< @brief Not applicable */
    FOC_Rs_update_Auto,   /*!< @brief Not applicable */
    numFOC_Rs_update_Select
} FOC_Rs_update_Select_e;

/**
  * @brief  Sensorless component handler definition
  */
typedef struct
{
  // IMPEDCORR_Obj *pImpedCorr;               /*!< @brief pointer on impedance correction component */
  HSO_Obj       *pHSO;                     /*!< @brief pointer on HSO component */

  FIXP_scaled_t Ls_Active_pu_fps;          /*!< @brief Ls active value in per unit */
  Voltages_Uab_t Uab_emf_pu;               /*!< @brief emf in alpha/beta coordinates */
  fixp30_t EmfMag_pu;                      /*!< @brief Store Emf magnitude */
  fixp30_t AngleCorrection_pu;             /*!< @brief Angle correction from ZeST to high Sensitivity observer */
  Vector_ab_t Flux_ab;                     /*!< @brief flux in alpha/beta coordinates */
  fixp30_t theta_pu;                       /*!< @brief Estimated electrical angle @f$\theta @f$ */
  fixp30_t Fe_hso_pu;                      /*!< @brief Estimated electrical frequency @f$\omega @f$ */
  fixp30_t Flux_Active_Wb;                 /*!< @brief Active flux */
  FIXP_scaled_t oneOverFlux_pu_fps;        /*!< @brief Stored 1/flux value */
  fixp30_t zestOutD;                       /*!< @brief ZeST outD signal */
  fixp30_t zestOutQ;                       /*!< @brief ZeST outQ signal */
  fixp30_t DirCheck_threshold_pu;          /*!< @brief threshold to flip angle in case ZeST is locked to the opposite angle */
  fixp30_t Hz_pu_to_step_per_isr;          /*!< @brief factor conversion from Hz to angle step */
  fixp30_t OpenLoopAngle;                  /*!< @brief open loop angle */
  float_t  MinCrossOverAtLowSpeed_Hz;      /*!< @brief 2nd possible min cross-over frequency when Zest fraction is abobe 80% */
  FOC_Rs_update_Select_e Rs_update_select; /*!< @brief selects the mode of the RS update */
  bool flagFreezeHsoAngle;                 /*!< @brief flag to freeze HSO angle */
  bool flagHsoAngleEqualsOpenLoopAngle;    /*!< @brief flag to copy open loop angle in the HSO */
  bool flagDisableImpedanceCorrection;     /*!< @brief Disable impedance correction update, used during profiling */
  bool closedLoopAngleEnabled;             /*!< @brief Use estimated angle feedback. Use internal open loop angle generator when disabled. */
  bool flagDynamicZestConfig;              /*!< @brief flag to enable dynamic ZeST feature. */

} SPD_Handle_t;

/**
  * @brief  Sensorless component input data definition
  */
typedef struct
{
  Currents_Iab_t Iab_pu;       /*!< @brief phase currents in alpha/beta coordinates */
  Voltages_Uab_t Uab_pu;       /*!< @brief phase voltages in alpha/beta coordinates*/
} Sensorless_Input_t;

/**
  * @brief  Sensorless component output data definition
  */
typedef struct
{
  Voltages_Uab_t Uab_emf_pu;       /*!< @brief alpha/beta emf in per-unit  */
  Currents_Idq_t Idq_inject_pu;    /*!< @brief Idq injected signal in per-unit */
  fixp30_t       speed;            /*!< @brief Estimated electrical motor speed */
  fixp30_t       angSpeed;         /*!< @brief Estimated electrical angle  */
} Sensorless_Output_t;

/* Initializes speed & position control component. */
void SPD_Init(SPD_Handle_t *pHandle, FLASH_Params_t const *flashParams);

/* Runs speed & position control component. */
void SPD_Run(SPD_Handle_t *pHandle, Sensorless_Input_t *In, Sensorless_Output_t *Out);

/* Exports the estimated electrical speeds. */
void SPD_GetSpeed(SPD_Handle_t *pHandle,  fixp30_t *emfSpeed, fixp30_t *speedLP);

/* Exports the estimated electrical angle. */
void SPD_GetAngle(SPD_Handle_t *pHandle,  fixp30_t *angle);

/* Updates the open loop angles. */
void SPD_UpdtOpenLoopAngle(SPD_Handle_t *pHandle, fixp30_t speedRef);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* _SPEEDNPOSFDBK_H_ */
