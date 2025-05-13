/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#include <stdio.h>
#include "GPIO.h"
#include "ADC.h"
#include "DMA.h"
#include "USB.h"
#include "TIM.h"
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define SENS_I_A_Pin GPIO_PIN_1
#define SENS_I_A_GPIO_Port GPIOC
#define SENS_I_B_Pin GPIO_PIN_2
#define SENS_I_B_GPIO_Port GPIOC
#define SENS_I_C_Pin GPIO_PIN_3
#define SENS_I_C_GPIO_Port GPIOC
#define SENS_V_A_Pin GPIO_PIN_1
#define SENS_V_A_GPIO_Port GPIOA
#define SENS_V_B_Pin GPIO_PIN_3
#define SENS_V_B_GPIO_Port GPIOA
#define ENABLE_Pin GPIO_PIN_4
#define ENABLE_GPIO_Port GPIOA
#define NSCS_Pin GPIO_PIN_4
#define NSCS_GPIO_Port GPIOC
#define SENS_V_C_Pin GPIO_PIN_0
#define SENS_V_C_GPIO_Port GPIOB
#define BTN_2_Pin GPIO_PIN_12
#define BTN_2_GPIO_Port GPIOB
#define BTN_2_EXTI_IRQn EXTI15_10_IRQn
#define TIM1_CH3N_Pin GPIO_PIN_13
#define TIM1_CH3N_GPIO_Port GPIOB
#define TIM1_CH1N_Pin GPIO_PIN_15
#define TIM1_CH1N_GPIO_Port GPIOB
#define TIM1_CH3_Pin GPIO_PIN_8
#define TIM1_CH3_GPIO_Port GPIOA
#define TIM1_CH1_Pin GPIO_PIN_10
#define TIM1_CH1_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_15
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_10
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_11
#define LED_3_GPIO_Port GPIOC
#define BTN_1_Pin GPIO_PIN_9
#define BTN_1_GPIO_Port GPIOB
#define BTN_1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
