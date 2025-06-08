/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc2;

/* USER CODE BEGIN Private defines */
#define SENS_I_A_Pin GPIO_PIN_1
#define SENS_I_A_GPIO_Port GPIOC
#define SENS_I_B_Pin GPIO_PIN_2
#define SENS_I_B_GPIO_Port GPIOC
#define SENS_I_C_Pin GPIO_PIN_3
#define SENS_I_C_GPIO_Port GPIOC

#define ADC_CURR_BUFFER_SIZE 3
#define ADC_VOLT_BUFFER_SIZE 3

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern uint16_t currentBuffer[ADC_CURR_BUFFER_SIZE];
extern uint16_t voltageBuffer[ADC_VOLT_BUFFER_SIZE];
// historia próbki BEMF do porównania
extern volatile uint16_t bemfLast;
extern bool start;
/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC_StartDMA_ADC();

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc);

void ADC_BemfZeroCrossing();

static inline uint16_t ADC_ValueToVoltage(uint16_t adcValue)
{
    return (adcValue * 3300) / 65535; 
}

/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

