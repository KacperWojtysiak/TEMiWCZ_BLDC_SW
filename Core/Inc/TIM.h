/**
  ******************************************************************************
  * @file           : TIM.h
  * @author         : 
  * @brief          : Header for all TIM related functionalities.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "stm32g4xx_hal.h"
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
/* PWM generation and current reading */
#define PWM_FREQUENCY                       10000
#define PWM_FREQ_SCALING                    1
// #define LOW_SIDE_SIGNALS_ENABLING           LS_PWM_TIMER
#define DEADTIME_NS                         565 /*!< Dead-time to be inserted by FW, only if low side signals are enabled */

#define ADV_TIM_CLK_MHz   170
#define TIM_CLOCK_DIVIDER 170
#define PWM_PERIOD_CYCLES (uint16_t)((ADV_TIM_CLK_MHz * 1000000u) / (TIM_CLOCK_DIVIDER * PWM_FREQUENCY)) //(uint16_t)(((uint32_t)ADV_TIM_CLK_MHz * (uint32_t)1000000u / ((uint32_t)(PWM_FREQUENCY))) & (uint16_t)0xFFFE)
#define PWM_DUTY_CYCLE                           50 // [%]
#define PWM_PULSE                                (PWM_DUTY_CYCLE * PWM_PERIOD_CYCLES / 100)

#define REGULATION_EXECUTION_RATE 1 /*!< FOC execution rate in number of PWM cycles */
#define REP_COUNTER               1 //(uint16_t)((REGULATION_EXECUTION_RATE * 2u) - 1u)

/* DEAD TIME CALCULATION */
#define DEAD_TIME_ADV_TIM_CLK_MHz           (ADV_TIM_CLK_MHz * TIM_CLOCK_DIVIDER)
#define DEAD_TIME_COUNTS_1                  (DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS / 1000uL)
#if     (DEAD_TIME_COUNTS_1 <= 255)
#define DEAD_TIME_COUNTS                    (uint16_t)DEAD_TIME_COUNTS_1
#elif   (DEAD_TIME_COUNTS_1 <= 508)
#define DEAD_TIME_COUNTS                    (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/2) /1000uL) + 128)
#elif   (DEAD_TIME_COUNTS_1 <= 1008)
#define DEAD_TIME_COUNTS                    (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/8) /1000uL) + 320)
#elif   (DEAD_TIME_COUNTS_1 <= 2015)
#define DEAD_TIME_COUNTS                    (uint16_t)(((DEAD_TIME_ADV_TIM_CLK_MHz * DEADTIME_NS/16) /1000uL) + 384)
#else
#define DEAD_TIME_COUNTS 510
#endif

#define OVS_COUNT                            (4) /*!< @brief  Oversampling level. default value is 4 */
#define M1_AUX_TIM_PERIOD_CYCLES            (uint16_t)((((uint32_t)ADV_TIM_CLK_MHz * (uint32_t)1000000u\
                                            / ((uint32_t)(PWM_FREQUENCY)*OVS_COUNT)))-1)

extern TIM_HandleTypeDef htim1;

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void TIM_StartTIM1();

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H */
