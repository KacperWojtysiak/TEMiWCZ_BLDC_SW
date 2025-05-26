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
    BADC_IsZcDetected(&bemfHandle, mc.step);
  }

  if (adc->Instance == ADC2){
    // ADC_CalcCurrent(currentBuffer);
    ITM_SendValue(1, (uint32_t)currentBuffer[0] );
    ITM_SendValue(2, (uint32_t)currentBuffer[1] );
    ITM_SendValue(3, (uint32_t)currentBuffer[2] );
  }
}

// void ADC_CalcCurrent(uint16_t* current){
//   uint16_t vref = 4095;
//   float gain = 5.0f;
//   float shuntR = 0.01f;

//   for (size_t i = 0; i < 3; i++)
//   {
//     // current[i] = (uint16_t)( (float)(vref - current[i]) / (gain * shuntR));
//     current[i] = (vref - current[i]) / (gain * shuntR);
//     __NOP();
//   }
  
// }

uint16_t* ADC_GetBufferADC1Ref() { return currentBuffer; }
