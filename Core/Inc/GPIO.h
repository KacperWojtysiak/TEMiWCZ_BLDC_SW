/**
  ******************************************************************************
  * @file           : GPIO.h
  * @author         : 
  * @brief          : Header for all GPIO related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
#define BTN_Pin_1             GPIO_PIN_9
#define BTN_Pin_2             GPIO_PIN_12
#define BTN_GPIO_Port         GPIOB
#define LED_Pin_1             GPIO_PIN_15
#define LED_Pin_2             GPIO_PIN_10
#define LED_Pin_3             GPIO_PIN_11
#define LED_GPIO_Port_C       GPIOC
#define LED_GPIO_Port_A       GPIOA

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void GPIO_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
