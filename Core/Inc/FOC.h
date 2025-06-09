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
#include "open_loop.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
extern OpenLoop_Handle_t *pOpenLoop[1];

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void FOC_Init();
void FOC_Clear(uint8_t bMotor);

void FOC_HighFrequencyTask();
void FOC_CalcCurrRef(uint8_t bMotor);

#ifdef __cplusplus
}
#endif

#endif /* __FOC_H */
