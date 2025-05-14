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
uint16_t currentBuffer[ADC_CURR_BUFFER_SIZE];
uint16_t voltageBuffer[ADC_VOLT_BUFFER_SIZE];

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC_StartDMA_ADC(){
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&voltageBuffer[0], ADC_VOLT_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)&currentBuffer[0], ADC_CURR_BUFFER_SIZE);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc){ // ADC interrupt no DMA
  if (adc->Instance == ADC1){
    // TODO ADC -> DMA (4 * uint32) currentBuffer full handle
    // uint32_t sum = 0;
    // for (uint8_t i = 0; i < ADC_CURR_BUFFER_SIZE; i++){
    //   sum += buffer[i];
    // }
    // ITM_SendValue(1, (uint32_t)(sum / ADC_CURR_BUFFER_SIZE) );
    // ITM_SendValue(1, (uint32_t)voltageBuffer[1] );
  }
  
  if (adc->Instance == ADC2){
    ITM_SendValue(1, (uint32_t)currentBuffer[0] );
    ITM_SendValue(2, (uint32_t)currentBuffer[1] );
    ITM_SendValue(3, (uint32_t)currentBuffer[2] );
  }
}

uint16_t* ADC_GetBufferADC1Ref() { return currentBuffer; }
