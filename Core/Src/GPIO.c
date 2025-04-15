/**
  ******************************************************************************
  * @file           : GPIO.c
  * @author         : 
  * @brief          : Code for all GPIO related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "GPIO.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    UNUSED(GPIO_Pin);
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void GPIO_Init(void)
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
  
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  
    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
  
    /*Configure GPIO pin : LD2_Pin */
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
  
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  }
