/**
  ******************************************************************************
  * @file           : DMA.c
  * @author         : 
  * @brief          : Code for all DMA related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "DMA.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
DMA_HandleTypeDef dmaAdc1;

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

DMA_HandleTypeDef* ADC_GetDMA1Ref() { return &dmaAdc1; }