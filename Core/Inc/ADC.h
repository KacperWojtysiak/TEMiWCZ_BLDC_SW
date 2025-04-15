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

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC1_Init(void);

void ADC_StartDMA_ADC1();
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc);

ADC_HandleTypeDef* ADC_GetADC1Ref();
uint32_t* ADC_GetBufferADC1Ref();

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
