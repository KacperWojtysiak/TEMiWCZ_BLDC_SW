/**
  ******************************************************************************
  * @file           : Scheduler.h
  * @author         : 
  * @brief          : Header for all Scheduler related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Scheduler_H
#define __Scheduler_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
typedef struct {
    uint16_t tick;
    uint8_t step;
    const uint16_t pwmDuty;
} McScheduler_t;

extern McScheduler_t mc;

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void Task();
void Task1ms();
void Task10ms();
void Task100ms();
void SixStepTask();

#ifdef __cplusplus
}
#endif

#endif /* __Scheduler_H */
