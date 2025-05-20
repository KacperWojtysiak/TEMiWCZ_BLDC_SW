/**
  ******************************************************************************
  * @file           : Scheduler.c
  * @author         : 
  * @brief          : Code for all Scheduler related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "Scheduler.h"
/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
static volatile uint16_t tick = 0;
static uint8_t step = 0;
const uint16_t pwm_duty = 200; // przykład wypełnienia PWM


/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void Task(){
    static volatile uint16_t tick = 1;
    
    if ( (tick % 2000) == 0){
        HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
        CDC_Transmit_FS((uint8_t*)"DUPA", 4);
        tick = 0;
    }
    if ( (tick % 100) == 0){
        HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    }
    if (tick % 100 == 0) {
        six_step_task();
    }
    tick ++;
}

void six_step_task(void) {
    switch(step) {
        case 0:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_duty);  // U+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 1:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_duty);  // U+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_duty);  // V-
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 2:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_duty);  // V+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 3:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_duty);  // V+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_duty);  // W-
            break;
        case 4:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_duty);  // W+
            break;
        case 5:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_duty);  // U-
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_duty);  // W+
            break;
    }
    step = (step + 1) % 6;
}
