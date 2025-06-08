/**
  ******************************************************************************
  * @file           : MC.c
  * @author         : 
  * @brief          : Code for all motor control related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "MC.h"
#include "mc_type.h"
#include "mc_config.h"

#include "FOC.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
static volatile uint8_t bMCBootCompleted = 0;

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/
void mc_lock_pins (void)
{
    LL_GPIO_LockPin(SENS_I_A_GPIO_Port, SENS_I_A_Pin);
    LL_GPIO_LockPin(SENS_I_B_GPIO_Port, SENS_I_B_Pin);
    LL_GPIO_LockPin(SENS_I_C_GPIO_Port, SENS_I_C_Pin);
    LL_GPIO_LockPin(M1_PWM_UH_GPIO_Port, M1_PWM_UH_Pin);
    LL_GPIO_LockPin(M1_PWM_VH_GPIO_Port, M1_PWM_VH_Pin);
    LL_GPIO_LockPin(M1_PWM_VL_GPIO_Port, M1_PWM_VL_Pin);
    LL_GPIO_LockPin(M1_PWM_WH_GPIO_Port, M1_PWM_WH_Pin);
    LL_GPIO_LockPin(M1_PWM_WL_GPIO_Port, M1_PWM_WL_Pin);
    LL_GPIO_LockPin(M1_PWM_UL_GPIO_Port, M1_PWM_UL_Pin);
}

__weak void MCboot( MCI_Handle_t* pMC)
{
  if (NULL == pMC)
  {
    return;
  }
  else
  {
    bMCBootCompleted = 0;
    FOC_Init();
    PID_HandleInit(&PIDSpeedHandle_M1);
    VSS_Init(&VirtualSpeedSensorM1);
    VVBS_Init(&BusVoltageSensor_M1);

    bMCBootCompleted = 1U;
  }
}

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void MC_Init(void){
  // Reconfigure the SysTick interrupt to fire every 500 us.
  (void)HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / SYS_TICK_FREQUENCY);
  HAL_NVIC_SetPriority(SysTick_IRQn, uwTickPrio, 0U);

/* Initialize the Motor Control Subsystem */
  MCboot(&Mci[M1]);
  mc_lock_pins();
}


