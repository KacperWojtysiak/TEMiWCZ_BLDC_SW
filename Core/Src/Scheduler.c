/**
  ******************************************************************************
  * @file           : Scheduler.c
  * @author         : 
  * @brief          : Code for all Scheduler related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "Scheduler.h"
#include "SPD.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
#define COMM_DELAY_US 200

static volatile uint16_t tick = 0;
McScheduler_t mc = {
    .step = 0,
    .pwmDuty = 150
};

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void Task1ms(){

}

void Task10ms(){
    HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
}

void Task100ms(){
    HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
    UpdateSpeedFromBEMF();
}

void Task(){
    static volatile uint16_t tick = 1;

    Task1ms();
    if ( (tick % 10) == 0){
        Task10ms();
    }
    if ( (tick % 100) == 0){
        CDC_Transmit_FS((uint8_t*)"DUPA", 4);
        Task100ms();
    }
    tick ++;
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        SixStepTask();
    }
}

void SixStepTask() {
    ConfigureSampling();
    switch(mc.step) {
        case 0:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mc.pwmDuty);  // U+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 1:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mc.pwmDuty);  // U+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mc.pwmDuty);  // V-
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 2:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mc.pwmDuty);  // V+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);         // W off
            break;
        case 3:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mc.pwmDuty);  // V+
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mc.pwmDuty);  // W-
            break;
        case 4:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);         // U off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mc.pwmDuty);  // W+
            break;
        case 5:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mc.pwmDuty);  // U-
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);         // V off
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mc.pwmDuty);  // W+
            break;
    }
    mc.step = (mc.step + 1) % 6;
}
