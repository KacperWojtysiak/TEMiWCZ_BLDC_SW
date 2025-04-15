/**
  ******************************************************************************
  * @file           : TIM.h
  * @author         : 
  * @brief          : Header for all TIM related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void TIM1_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void TIM_StartTIM1();

TIM_HandleTypeDef* TIM_GetTim1Ref();

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H */
