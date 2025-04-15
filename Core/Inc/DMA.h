/**
  ******************************************************************************
  * @file           : DMA.h
  * @author         : 
  * @brief          : Header for all DMA related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DMA_H
#define __DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"
#include "ADC.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void DMA_Init(void);

DMA_HandleTypeDef* ADC_GetDMA1Ref();

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H */
