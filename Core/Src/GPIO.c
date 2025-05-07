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
    // HAL_GPIO_TogglePin(LD1_GPIO_Port, LD2_Pin);
    HAL_GPIO_TogglePin(LED_GPIO_Port_A, LED_Pin_1);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_2);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_3);
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
    // HAL_GPIO_WritePin(LD1_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO_Port_A, LED_Pin_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO_Port_C, LED_Pin_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GPIO_Port_C, LED_Pin_3, GPIO_PIN_RESET);

    // ENABLE pin
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  
    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = BTN_Pin_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BTN_Pin_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);
  
    /*Configure GPIO pin : LD2_Pin */
    // GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // HAL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LED_Pin_1;
    HAL_GPIO_Init(LED_GPIO_Port_A, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_Pin_2;
    HAL_GPIO_Init(LED_GPIO_Port_C, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LED_Pin_3;
    HAL_GPIO_Init(LED_GPIO_Port_C, &GPIO_InitStruct);

    //ENABLE 
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  }
