/**
  ******************************************************************************
  * @file           : FOC.h
  * @author         : 
  * @brief          : Header for all FOC related functionalities.
  ******************************************************************************
  */
#ifndef __FOC_H
#define __FOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void FOC_Init();
void FOC_Clear(uint8_t bMotor);


#ifdef __cplusplus
}
#endif

#endif /* __FOC_H */
