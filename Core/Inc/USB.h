/**
  ******************************************************************************
  * @file           : USB.h
  * @author         : 
  * @brief          : Header for all USB related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_H
#define __USB_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void USB_PCD_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __USB_H */
