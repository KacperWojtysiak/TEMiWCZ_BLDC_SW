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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FUNC_BUTTON_2_Pin GPIO_PIN_13
#define FUNC_BUTTON_2_GPIO_Port GPIOC
#define NRST_Pin GPIO_PIN_10
#define NRST_GPIO_Port GPIOG
#define PVDDSENSE_Pin GPIO_PIN_0
#define PVDDSENSE_GPIO_Port GPIOC
#define SENS_I_A_Pin GPIO_PIN_1
#define SENS_I_A_GPIO_Port GPIOC
#define SENS_I_B_Pin GPIO_PIN_2
#define SENS_I_B_GPIO_Port GPIOC
#define SENS_I_C_Pin GPIO_PIN_3
#define SENS_I_C_GPIO_Port GPIOC
#define SENS_VN_Pin GPIO_PIN_0
#define SENS_VN_GPIO_Port GPIOA
#define SENS_V_A_Pin GPIO_PIN_1
#define SENS_V_A_GPIO_Port GPIOA
#define SENS_VNA2_Pin GPIO_PIN_2
#define SENS_VNA2_GPIO_Port GPIOA
#define SENS_V_B_Pin GPIO_PIN_3
#define SENS_V_B_GPIO_Port GPIOA
#define ENABLE_Pin GPIO_PIN_4
#define ENABLE_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_5
#define SCLK_GPIO_Port GPIOA
#define SDO_Pin GPIO_PIN_6
#define SDO_GPIO_Port GPIOA
#define SDI_Pin GPIO_PIN_7
#define SDI_GPIO_Port GPIOA
#define nSCS_Pin GPIO_PIN_4
#define nSCS_GPIO_Port GPIOC
#define CAL_Pin GPIO_PIN_5
#define CAL_GPIO_Port GPIOC
#define SENS_V_C_Pin GPIO_PIN_0
#define SENS_V_C_GPIO_Port GPIOB
#define TEMP_MOSFET_Pin GPIO_PIN_1
#define TEMP_MOSFET_GPIO_Port GPIOB
#define SENS_VNB2_Pin GPIO_PIN_2
#define SENS_VNB2_GPIO_Port GPIOB
#define nFAULT_Pin GPIO_PIN_10
#define nFAULT_GPIO_Port GPIOB
#define INLA_Pin GPIO_PIN_13
#define INLA_GPIO_Port GPIOB
#define INLB_Pin GPIO_PIN_14
#define INLB_GPIO_Port GPIOB
#define INLC_Pin GPIO_PIN_15
#define INLC_GPIO_Port GPIOB
#define INHA_Pin GPIO_PIN_8
#define INHA_GPIO_Port GPIOA
#define INHB_Pin GPIO_PIN_9
#define INHB_GPIO_Port GPIOA
#define INHC_Pin GPIO_PIN_10
#define INHC_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_15
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_10
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_11
#define LED_3_GPIO_Port GPIOC
#define RELAY_Pin GPIO_PIN_12
#define RELAY_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BOOT0_Pin GPIO_PIN_8
#define BOOT0_GPIO_Port GPIOB
#define FUNC_BUTTON_Pin GPIO_PIN_9
#define FUNC_BUTTON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
