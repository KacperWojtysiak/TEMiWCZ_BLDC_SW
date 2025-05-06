/**
  ******************************************************************************
  * @file           : ADC.h
  * @author         : 
  * @brief          : Header for all ADC related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
#define SENS_I_A_Pin GPIO_PIN_1
#define SENS_I_A_GPIO_Port GPIOC
#define SENS_I_B_Pin GPIO_PIN_2
#define SENS_I_B_GPIO_Port GPIOC
#define SENS_I_C_Pin GPIO_PIN_3
#define SENS_I_C_GPIO_Port GPIOC

#define ADC_BUFFER_SIZE 1U
#define ADC_DMA_PRIORITY 1
/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC1_Init(void);

void ADC_StartDMA_ADC1();
void MX_ADC2_Init(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc);

ADC_HandleTypeDef* ADC_GetADC1Ref();
uint16_t* ADC_GetBufferADC1Ref();

static inline uint16_t ADC_ValueToVoltage(uint16_t adcValue)
{
    return (adcValue * 3300) / 65535; 
}


#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
