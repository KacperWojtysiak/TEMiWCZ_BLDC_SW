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


#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
