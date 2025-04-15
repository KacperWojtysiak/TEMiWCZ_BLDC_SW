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
#define ADC_BUFFER_SIZE 4U

ADC_HandleTypeDef adc1;
uint32_t* buffer;

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC1_Init(void){
   ADC_MultiModeTypeDef multimode = {0};
   ADC_ChannelConfTypeDef sConfig = {0};

   /** Common config
   */
   adc1.Instance = ADC1;
   adc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
   adc1.Init.Resolution = ADC_RESOLUTION_12B;
   adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   adc1.Init.GainCompensation = 0;
   adc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
   adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   adc1.Init.LowPowerAutoWait = DISABLE;
   adc1.Init.ContinuousConvMode = ENABLE;
   adc1.Init.NbrOfConversion = 1;
   adc1.Init.DiscontinuousConvMode = DISABLE;
   adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   adc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   adc1.Init.DMAContinuousRequests = DISABLE;
   adc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
   adc1.Init.OversamplingMode = DISABLE;
   if (HAL_ADC_Init(&adc1) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure the ADC multi-mode
   */
   multimode.Mode = ADC_MODE_INDEPENDENT;
   if (HAL_ADCEx_MultiModeConfigChannel(&adc1, &multimode) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure Regular Channel
   */
   sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   sConfig.SingleDiff = ADC_SINGLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   if (HAL_ADC_ConfigChannel(&adc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
 }

void ADC_StartDMA_ADC1(){
  HAL_ADC_Start_DMA(&adc1, buffer, ADC_BUFFER_SIZE);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc){
  if (adc->Instance == ADC1){
    uint32_t val = HAL_ADC_GetValue(adc);
    UNUSED(val);
  }
}
  
ADC_HandleTypeDef* ADC_GetADC1Ref() { return &adc1; }
uint32_t* ADC_GetBufferADC1Ref() { return buffer; }
