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
#define B1_Pin                GPIO_PIN_13
#define B1_GPIO_Port          GPIOC
#define LD2_Pin               GPIO_PIN_5
#define LD2_GPIO_Port         GPIOA
#define RCC_OSC_IN_Pin        GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port  GPIOF
#define RCC_OSC_OUT_Pin       GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define LPUART1_TX_Pin        GPIO_PIN_2
#define LPUART1_TX_GPIO_Port  GPIOA
#define LPUART1_RX_Pin        GPIO_PIN_3
#define LPUART1_RX_GPIO_Port  GPIOA
#define T_SWDIO_Pin           GPIO_PIN_13
#define T_SWDIO_GPIO_Port     GPIOA
#define T_SWCLK_Pin           GPIO_PIN_14
#define T_SWCLK_GPIO_Port     GPIOA
#define T_SWO_Pin             GPIO_PIN_3
#define T_SWO_GPIO_Port       GPIOB

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void GPIO_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
