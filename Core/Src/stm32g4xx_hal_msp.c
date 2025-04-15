/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32g4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern DMA_HandleTypeDef dmaAdc1;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  HAL_PWREx_DisableUCPDDeadBattery();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hadc->Instance==ADC1)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_ADC12_CLK_ENABLE();

    /* ADC1 DMA Init */
    /* ADC1 Init */
    dmaAdc1.Instance = DMA1_Channel1;
    dmaAdc1.Init.Request = DMA_REQUEST_ADC1;
    dmaAdc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dmaAdc1.Init.PeriphInc = DMA_PINC_DISABLE;
    dmaAdc1.Init.MemInc = DMA_MINC_ENABLE;
    dmaAdc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dmaAdc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    dmaAdc1.Init.Mode = DMA_NORMAL;
    dmaAdc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&dmaAdc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hadc,DMA_Handle,dmaAdc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC12_CLK_DISABLE();

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(hadc->DMA_Handle);
    /* ADC1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
  }
}

/**
  * @brief UART MSP Initialization
  * This function configures the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(huart->Instance==LPUART1)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**LPUART1 GPIO Configuration
    PA2     ------> LPUART1_TX
    PA3     ------> LPUART1_RX
    */
    GPIO_InitStruct.Pin = LPUART1_TX_Pin|LPUART1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF12_LPUART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

/**
  * @brief UART MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==LPUART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();

    /**LPUART1 GPIO Configuration
    PA2     ------> LPUART1_TX
    PA3     ------> LPUART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, LPUART1_TX_Pin|LPUART1_RX_Pin);
  }

}

/**
  * @brief TIM_Base MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
 void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
 {
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   if(htim_base->Instance==TIM1)
   {
     /* Peripheral clock enable */
     __HAL_RCC_TIM1_CLK_ENABLE();
 
     __HAL_RCC_GPIOB_CLK_ENABLE();
     __HAL_RCC_GPIOA_CLK_ENABLE();
     /**TIM1 GPIO Configuration
     PB10     ------> TIM1_BKIN
     PA9     ------> TIM1_CH2
     PA10     ------> TIM1_CH3
     */
     GPIO_InitStruct.Pin = BRK_TIM1_Pin;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
     GPIO_InitStruct.Pull = GPIO_PULLUP;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
     GPIO_InitStruct.Alternate = GPIO_AF12_TIM1_COMP1;
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
     GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
     GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 
     /* TIM1 interrupt Init */
     HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
     HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
     HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
   }
 
 }
 
 void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
 {
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   if(htim->Instance==TIM1)
   {
     __HAL_RCC_GPIOB_CLK_ENABLE();
     __HAL_RCC_GPIOA_CLK_ENABLE();
     /**TIM1 GPIO Configuration
     PB13     ------> TIM1_CH1N
     PB14     ------> TIM1_CH2N
     PB15     ------> TIM1_CH3N
     PA8     ------> TIM1_CH1
     */
     GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Pull = GPIO_PULLDOWN;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
     GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
     GPIO_InitStruct.Pin = GPIO_PIN_8;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Pull = GPIO_PULLDOWN;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
     GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   }
 
 }
 /**
   * @brief TIM_Base MSP De-Initialization
   * This function freeze the hardware resources used in this example
   * @param htim_base: TIM_Base handle pointer
   * @retval None
   */
 void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
 {
   if(htim_base->Instance==TIM1)
   {
     /* Peripheral clock disable */
     __HAL_RCC_TIM1_CLK_DISABLE();
 
     /**TIM1 GPIO Configuration
     PB10     ------> TIM1_BKIN
     PB13     ------> TIM1_CH1N
     PB14     ------> TIM1_CH2N
     PB15     ------> TIM1_CH3N
     PA8     ------> TIM1_CH1
     PA9     ------> TIM1_CH2
     PA10     ------> TIM1_CH3
     */
     HAL_GPIO_DeInit(GPIOB, BRK_TIM1_Pin|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
 
     /* TIM1 interrupt DeInit */
     HAL_NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);
     HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
     HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
     /* USER CODE BEGIN TIM1_MspDeInit 1 */
 
     /* USER CODE END TIM1_MspDeInit 1 */
   }
 
 }
 
/**
  * @brief PCD MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hpcd->Instance==USB)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }
    /* Peripheral clock enable */
    __HAL_RCC_USB_CLK_ENABLE();
  }

}

/**
  * @brief PCD MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USB_CLK_DISABLE();
  }

}
