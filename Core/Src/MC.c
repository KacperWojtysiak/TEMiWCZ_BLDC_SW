/**
  ******************************************************************************
  * @file           : MC.c
  * @author         : 
  * @brief          : Code for all motor control related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "MC.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
static volatile uint8_t bMCBootCompleted = ((uint8_t)0);

MCI_Handle_t mc =
{
    // .pSTC = &SpeednTorqCtrlM1,
    // .pFOCVars = &FOCVars[0],
    // .pVSS = &VirtualSpeedSensorM1,
    // .pPWM = &PWM_Handle_M1._Super,
    .lastCommand = MCI_NOCOMMANDSYET,
    .hFinalSpeed = 0,
    .hFinalTorque = 0,
    // .pScale = &scaleParams_M1,
    .hDurationms = 0,
    .DirectCommand = MCI_NO_COMMAND,
    .State = IDLE,
    .CurrentFaults = MC_NO_FAULTS,
    .PastFaults = MC_NO_FAULTS,
    .CommandState = MCI_BUFFER_EMPTY,
};

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
    bMCBootCompleted = (uint8_t )0;

    /*************************************************/
    /*    FOC initialization         */
    /*************************************************/
    // pMC[M1] = &Mci[M1];
    // FOC_Init();

    // ASPEP_start(&aspepOverUartA);
    /* USER CODE BEGIN MCboot 1 */

    /* USER CODE END MCboot 1 */

    /******************************************************/
    /*   PID component initialization: speed regulation   */
    /******************************************************/
    // PID_HandleInit(&PIDSpeedHandle_M1);

    /****************************************************/
    /*   Virtual speed sensor component initialization  */
    /****************************************************/
    // VSS_Init(&VirtualSpeedSensorM1);

    /**********************************************************/
    /*   Virtual bus voltage sensor component initialization  */
    /**********************************************************/
    // VVBS_Init(&BusVoltageSensor_M1);

    /*******************************************************/
    /*   Temperature measurement component initialization  */
    /*******************************************************/
    // NTC_Init(&TempSensor_M1);

    /* Applicative hook in MCBoot() */
    // MC_APP_BootHook();

    /* USER CODE BEGIN MCboot 2 */

    /* USER CODE END MCboot 2 */

    bMCBootCompleted = 1U;
  }
}

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void MC_Init(void){
  // Reconfigure the SysTick interrupt to fire every 500 us.
  (void)HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / SYS_TICK_FREQUENCY);
  HAL_NVIC_SetPriority(SysTick_IRQn, uwTickPrio, 0U);

/* Initialize the Motor Control Subsystem */
  MCboot(&mc);
  mc_lock_pins();
}


