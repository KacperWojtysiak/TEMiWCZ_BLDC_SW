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
#include "usbd_cdc_if.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void USB_Send();

#ifdef __cplusplus
}
#endif

#endif /* __USB_H */
