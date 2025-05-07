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
ADC_HandleTypeDef adc1;
ADC_HandleTypeDef hadc2;
uint16_t buffer[ADC_BUFFER_SIZE];

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void ADC1_Init(void){
   ADC_MultiModeTypeDef multimode = {0};
   ADC_ChannelConfTypeDef sConfig = {0};

   /** Common config */
   adc1.Instance = ADC1;
   adc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; //ADC_CLOCK_ASYNC_DIV1
   adc1.Init.Resolution = ADC_RESOLUTION_12B;
   adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; //ADC_DATAALIGN_LEFT
   adc1.Init.GainCompensation = 0;
   adc1.Init.ScanConvMode = ADC_SCAN_DISABLE; // ADC_SCAN_ENABLE
   adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // x ADC_EOC_SEQ_CONV
   adc1.Init.LowPowerAutoWait = DISABLE;
   adc1.Init.ContinuousConvMode = DISABLE;
   adc1.Init.NbrOfConversion = 1; // 3 TODO
   adc1.Init.DiscontinuousConvMode = DISABLE;
   adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; //ADC_EXTERNALTRIG_T3_TRGO // Wyzwolone timerem wewnętrznym nie do pwm!
   adc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; //ADC_EXTERNALTRIGCONVEDGE_FALLING
   adc1.Init.DMAContinuousRequests = ENABLE; //DISABLE
   adc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; //ADC_OVR_DATA_PRESERVED // Zachowuje dane!
   adc1.Init.OversamplingMode = DISABLE;
   if (HAL_ADC_Init(&adc1) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure the ADC multi-mode */
   multimode.Mode = ADC_MODE_INDEPENDENT;
   if (HAL_ADCEx_MultiModeConfigChannel(&adc1, &multimode) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure Regular Channel */
   sConfig.Channel = ADC_CHANNEL_7;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5; //ADC_SAMPLETIME_6CYCLES_5
   sConfig.SingleDiff = ADC_SINGLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   if (HAL_ADC_ConfigChannel(&adc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   
  // /** Configure Regular Channel */
  // sConfig.Channel = ADC_CHANNEL_8;
  // sConfig.Rank = ADC_REGULAR_RANK_2;
  // if (HAL_ADC_ConfigChannel(&adc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  /** Configure Regular Channel */
  // sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
  // sConfig.Rank = ADC_REGULAR_RANK_3;
  // if (HAL_ADC_ConfigChannel(&adc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }
 }
 
void MX_ADC2_Init(void)
 {
   ADC_ChannelConfTypeDef sConfig = {0};
   /** Common config
   */
   hadc2.Instance = ADC2;
   hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
   hadc2.Init.Resolution = ADC_RESOLUTION_12B;
   hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   hadc2.Init.GainCompensation = 0;
   hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
   hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   hadc2.Init.LowPowerAutoWait = DISABLE;
   hadc2.Init.ContinuousConvMode = DISABLE;
   hadc2.Init.NbrOfConversion = 1;
   hadc2.Init.DiscontinuousConvMode = DISABLE;
   hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc2.Init.DMAContinuousRequests = DISABLE;
   hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
   hadc2.Init.OversamplingMode = DISABLE;
   if (HAL_ADC_Init(&hadc2) != HAL_OK)
   {
     Error_Handler();
   }
 
   /** Configure Regular Channel
   */
   sConfig.Channel = ADC_CHANNEL_9;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   sConfig.SingleDiff = ADC_SINGLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
 }

void ADC_StartDMA_ADC1(){
  HAL_ADC_Start_DMA(&adc1, (uint32_t*)&buffer[0], (ADC_BUFFER_SIZE * sizeof(uint16_t)));
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adc){
  if (adc->Instance == ADC1){
    // TODO ADC -> DMA (4 * uint32) buffer full handle
    HAL_DMA_StateTypeDef dmaState = HAL_DMA_STATE_RESET;
    dmaState = HAL_DMA_GetState(ADC_GetDMA1Ref());
  }
}
  
ADC_HandleTypeDef* ADC_GetADC1Ref() { return &adc1; }
uint16_t* ADC_GetBufferADC1Ref() { return buffer; }
