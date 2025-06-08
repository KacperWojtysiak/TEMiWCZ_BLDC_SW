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
volatile uint16_t tick = 1;
volatile uint8_t startupCounter = 0;

McScheduler_t mc = {
    .step = 0,
    .pwmDuty = 100,
    .enabled = false
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
    // CDC_Transmit_FS((uint8_t*)"DUPA", 4);
    SPD_UpdateSpeedFromBEMF();
}

void Task(){
    Task1ms();
    if ( (tick % 10) == 0){
        Task10ms();
    }
    if ( (tick % 100) == 0){
        Task100ms();
    }
    if (tick % 50 == 0 && mc.enabled) {
    }
    tick ++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    HAL_GPIO_TogglePin(LED_GPIO_Port_A, LED_Pin_1);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_2);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_3);

    if (GPIO_Pin == BTN_1_Pin)
    {
        mc.enabled = !mc.enabled;
        if (mc.enabled) {
            // bemfHandle.IsLoopClosed = true;//false;
            mc.step = 0;
            bemfLast = 0;
            // BADC_Clear(&bemfHandle);
            // BADC_SetDirection(&bemfHandle, 1);
            startupCounter = 0;
        } else {
            // zatrzymaj PWM (wszystkie kanały OFF)
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        }
    }
}