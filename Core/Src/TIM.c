/**
  ******************************************************************************
  * @file           : TIM.c
  * @author         : 
  * @brief          : Code for all TIM related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "TIM.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/
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
#define REP_COUNTER               (uint16_t)((REGULATION_EXECUTION_RATE * 2u) - 1u)

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

TIM_HandleTypeDef htim1;

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
void TIM1_Init(void){
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
  TIMEx_BreakInputConfigTypeDef sBreakInputConfig = {0};
  // Base TIM1 setup
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = (TIM_CLOCK_DIVIDER - 1);
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = (PWM_PERIOD_CYCLES - 1);
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = REP_COUNTER;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  // Set TIM1 clock source, init PWM mode
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  // Disable timer synchronization and trigger outputs (standalone operation)
  {
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }
  }
  // Enable break input on BKIN pin (DRIVER OVERCURRENT PROTECTION)
  sBreakInputConfig.Source = TIM_BREAKINPUTSOURCE_BKIN;
  sBreakInputConfig.Enable = TIM_BREAKINPUTSOURCE_ENABLE;
  sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_LOW; // Activate when LOW!
  if (HAL_TIMEx_ConfigBreakInput(&htim1, TIM_BREAKINPUT_BRK, &sBreakInputConfig) != HAL_OK)
  {
    Error_Handler();
  }
  // Channels configuration
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = PWM_PULSE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  // Configure break / dead-time feature
  {
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = ((DEAD_TIME_COUNTS) / 2);
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 3;
    sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
    sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter = 0;
    sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
      Error_Handler();
    }
  }
  HAL_TIM_MspPostInit(&htim1);
}

void TIM_StartTIM1(){
  HAL_TIMEx_PWMN_Start_IT(&htim1, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
  UNUSED(htim);
}

void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim){
  UNUSED(htim);
  // TODO Overcurrent protection
}


TIM_HandleTypeDef* TIM_GetTim1Ref() { return &htim1; }