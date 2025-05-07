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
    HAL_GPIO_TogglePin(LED_GPIO_Port_A, LED_Pin_1);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_2);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_3);
}

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
