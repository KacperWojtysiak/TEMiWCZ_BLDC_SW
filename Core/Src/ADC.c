/**
  ******************************************************************************
  * @file           : ADC.c
  * @author         : 
  * @brief          : Code for all ADC related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "ADC.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
uint16_t buffer[ADC_BUFFER_SIZE];

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC_StartDMA_ADC1(){
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&buffer[0], (ADC_BUFFER_SIZE * sizeof(uint16_t)));
}

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc){ // ADC interrupt no DMA
//   if (adc->Instance == ADC1){
//     // TODO ADC -> DMA (4 * uint32) buffer full handle
//   }
// }

// HAL_ADC_ConvHalfCpltCallback()

uint16_t* ADC_GetBufferADC1Ref() { return buffer; }
