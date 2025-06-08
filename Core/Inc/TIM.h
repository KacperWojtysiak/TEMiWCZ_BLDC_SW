/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN Private defines */
/* PWM generation and current reading */
// #define PWM_FREQUENCY                       2000
#define PWM_FREQ_SCALING                    1
// #define LOW_SIDE_SIGNALS_ENABLING           LS_PWM_TIMER
// #define DEADTIME_NS                         565 /*!< Dead-time to be inserted by FW, only if low side signals are enabled */

#define ADV_TIM_CLK_MHz   170
// #define TIM_CLOCK_DIVIDER 170
// #define PWM_PERIOD_CYCLES (uint16_t)((ADV_TIM_CLK_MHz * 1000000u) / (TIM_CLOCK_DIVIDER * PWM_FREQUENCY)) //(uint16_t)(((uint32_t)ADV_TIM_CLK_MHz * (uint32_t)1000000u / ((uint32_t)(PWM_FREQUENCY))) & (uint16_t)0xFFFE)
#define PWM_DUTY_CYCLE                           0 // [%]
#define PWM_PULSE                                (PWM_DUTY_CYCLE * PWM_PERIOD_CYCLES / 100)

#define REGULATION_EXECUTION_RATE 1 /*!< FOC execution rate in number of PWM cycles */
#define REP_COUNTER               (uint16_t)((REGULATION_EXECUTION_RATE * 2u) - 1u)

#define OVS_COUNT                            (4) /*!< @brief  Oversampling level. default value is 4 */
#define M1_AUX_TIM_PERIOD_CYCLES            (uint16_t)((((uint32_t)ADV_TIM_CLK_MHz * (uint32_t)1000000u\
                                            / ((uint32_t)(PWM_FREQUENCY)*OVS_COUNT)))-1)

/* USER CODE END Private defines */

void MX_TIM1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void TIM_StartTIM1();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

