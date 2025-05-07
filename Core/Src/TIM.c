/**
  ******************************************************************************
  * @file           : TIM.c
  * @author         : 
  * @brief          : Code for all TIM related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "TIM.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void TIM_StartTIM1(){
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
  // HAL_TIMEx_PWMN_Start_IT(&htim1, TIM_CHANNEL_1);

  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
  // HAL_TIMEx_PWMN_Start_IT(&htim1, TIM_CHANNEL_2);

  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
  // HAL_TIMEx_PWMN_Start_IT(&htim1, TIM_CHANNEL_3);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
  UNUSED(htim);
}

void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){
  UNUSED(htim);
  // TODO Overcurrent protection
}


TIM_HandleTypeDef* TIM_GetTim1Ref() { return &htim1; }