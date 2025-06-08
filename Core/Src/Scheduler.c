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
volatile uint16_t tick = 1;

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
}

void Task(){
    Task1ms();
    if ( (tick % 10) == 0){
        Task10ms();
    }
    if ( (tick % 100) == 0){
        Task100ms();
    }
    tick ++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    HAL_GPIO_TogglePin(LED_GPIO_Port_A, LED_Pin_1);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_2);
    HAL_GPIO_TogglePin(LED_GPIO_Port_C, LED_Pin_3);

    if (GPIO_Pin == BTN_1_Pin)
    {
        // if (pMCI[M1]->State == STOP || pMCI[M1]->State == IDLE ){
        //     MC_StartMotor1();
        // }else{
        //     MC_StopMotor1();
        // }
    }
}