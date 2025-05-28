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
volatile uint16_t bemfLast = 0;
bool start = true; // Flaga do wykrywania zerowego przekroczenia BEMF

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC_StartDMA_ADC(){
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&voltageBuffer[0], ADC_VOLT_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)&currentBuffer[0], ADC_CURR_BUFFER_SIZE);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc){ // ADC interrupt no DMA
  if (adc->Instance == ADC1){
    // ITM_SendValue(1, (uint32_t)voltageBuffer[0] );
    // ITM_SendValue(2, (uint32_t)voltageBuffer[1] );
    // ITM_SendValue(3, (uint32_t)voltageBuffer[2] );
    if ( start ){
      BADC_IsZcDetected(&bemfHandle, mc.step);
    }
    // }else{
    //   // ADC_BemfZeroCrossing();
    // }
  }

  if (adc->Instance == ADC2){
    // ADC_CalcCurrent(currentBuffer);
    // ITM_SendValue(1, (uint32_t)currentBuffer[0] );
    // ITM_SendValue(2, (uint32_t)currentBuffer[1] );
    // ITM_SendValue(3, (uint32_t)currentBuffer[2] );
  }
}

void ADC_BemfZeroCrossing(){
  if (!bemfHandle.IsLoopClosed || bemfHandle.SpeedTimerState != COMMUTATION){
    return;
  }
  uint8_t idx = bemfChannelForStep[mc.step];
  uint16_t bemfNow = voltageBuffer[ idx ];

  const uint16_t zcRef = 2048;  // Zakładamy VREF=3.3V, dzielnik ≈ 0.175
  int16_t diffNow = (int16_t)bemfNow - zcRef;
  int16_t diffLast = (int16_t)bemfLast - zcRef;

  if ((diffNow * diffLast) < 0) {
    BADC_IsZcDetected(&bemfHandle, mc.step);
  }
  bemfLast = bemfNow;
}

uint16_t* ADC_GetBufferADC1Ref() { return currentBuffer; }
