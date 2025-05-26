
/**
  ******************************************************************************
  * @file    mc_parameters.c
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file provides definitions of HW parameters specific to the
  *          configuration of the subsystem.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//cstat -MISRAC2012-Rule-21.1
#include "main.h" //cstat !MISRAC2012-Rule-21.1
//cstat +MISRAC2012-Rule-21.1
#include "parameters_conversion.h"
// #include "r3_g4xx_pwm_curr_fdbk.h"

/* USER CODE BEGIN Additional include */

/* USER CODE END Additional include */

#define FREQ_RATIO 1                /* Dummy value for single drive */
#define FREQ_RELATION HIGHEST_FREQ  /* Dummy value for single drive */

/**
  * @brief  Current sensor parameters Motor 1 - three shunt - G4 HSO
  */
const R3_Params_t R3_ParamsM1 =
{

/* Current reading A/D Conversions initialization -----------------------------*/
  .ADCx_1           = ADC3,
  .ADCx_2           = ADC4,
  .ADCx_3           = ADC5,
  /* PWM generation parameters --------------------------------------------------*/
  .RepetitionCounter = REP_COUNTER,
  .TIMx              = TIM8,
  .TIMx_Oversample   = TIM3,
/* Internal OPAMP common settings --------------------------------------------*/
  .OPAMPParams     = MC_NULL,
/* Internal COMP settings ----------------------------------------------------*/
  .CompOCPASelection     = MC_NULL,
  .CompOCPAInvInput_MODE = NONE,
  .CompOCPBSelection     = MC_NULL,
  .CompOCPBInvInput_MODE = NONE,
  .CompOCPCSelection     = MC_NULL,
  .CompOCPCInvInput_MODE = NONE,
  .DAC_OCP_ASelection    = MC_NULL,
  .DAC_OCP_BSelection    = MC_NULL,
  .DAC_OCP_CSelection    = MC_NULL,
  .DAC_Channel_OCPA      = (uint32_t)0,
  .DAC_Channel_OCPB      = (uint32_t)0,
  .DAC_Channel_OCPC      = (uint32_t)0,
  .CompOVPSelection      = MC_NULL,
  .CompOVPInvInput_MODE  = NONE,
  .DAC_OVP_Selection     = MC_NULL,
  .DAC_Channel_OVP       = (uint32_t)0,
/* DAC settings --------------------------------------------------------------*/
  .DAC_OCP_Threshold     = 0,
  .DAC_OVP_Threshold     = 23830,
          .DMA_ADCx_1 = DMA2_Channel1,
  .DMA_ADCx_2 = DMA2_Channel2,
  .DMA_ADCx_3 = DMA2_Channel3,
};

const FLASH_Params_t  flashParams =
{
  .motor = {
    .polePairs = POLE_PAIR_NUM,
    .ratedFlux = MOTOR_RATED_FLUX,
    .rs = RS,
    .rsSkinFactor = MOTOR_RS_SKINFACTOR,
    .ls = LS,
    .maxCurrent = NOMINAL_CURRENT_A,
    .mass_copper_kg = MOTOR_MASS_COPPER_KG,
    .cooling_tau_s = MOTOR_COOLINGTAU_S,
    .name = MOTOR_NAME,
  },
  .polPulse =
  {
    .N  = NB_PULSE_PERIODS,
    .Nd = NB_DECAY_PERIODS,
    .PulseCurrentGoal = PULSE_CURRENT_GOAL,
  },
  .zest =
  {
    .zestThresholdFreqHz = 0,
    .zestInjectFreq = 0,
    .zestInjectD = 0,
    .zestGainD = 0,
    .zestGainQ = 0,
  },
  .PIDSpeed =
  {
    .pidSpdKp = PID_SPD_KP,
    .pidSpdKi = PID_SPD_KI,
  },
  .board =
  {
   .limitOverVoltage = BOARD_LIMIT_OVERVOLTAGE,
   .limitRegenHigh = BOARD_LIMIT_REGEN_HIGH,
   .limitRegenLow = BOARD_LIMIT_REGEN_LOW,
   .limitAccelHigh = BOARD_LIMIT_ACCEL_HIGH,
   .limitAccelLow = BOARD_LIMIT_ACCEL_LOW,
   .limitUnderVoltage = BOARD_LIMIT_UNDERVOLTAGE,
   .maxModulationIndex = BOARD_MAX_MODULATION,
   .softOverCurrentTrip = BOARD_SOFT_OVERCURRENT_TRIP,
},
  .KSampleDelay = KSAMPLE_DELAY,
  .throttle =
  {
    .offset = THROTTLE_OFFSET, /*TODO: Tobe defined */
	  .gain = THROTTLE_GAIN, /*TODO: Tobe defined */
	  .speedMaxRPM = THROTTLE_SPEED_MAX_RPM,
    .direction = 1,
  },
  .scale =
  {
    .voltage = VOLTAGE_SCALE,
    .current = CURRENT_SCALE,
    .frequency = FREQUENCY_SCALE
  },
};

const MotorConfig_reg_t *motorParams = &flashParams.motor;
const zestFlashParams_t *zestParams = &flashParams.zest;
const boardFlashParams_t *boardParams = &flashParams.board;
const scaleFlashParams_t *scaleParams = &flashParams.scale;
const throttleParams_t *throttleParams = &flashParams.throttle;
const float *KSampleDelayParams = &flashParams.KSampleDelay;
const PIDSpeedFlashParams_t *PIDSpeedParams = &flashParams.PIDSpeed;

/* USER CODE BEGIN Additional parameters */

/* USER CODE END Additional parameters */

/******************* (C) COPYRIGHT 2024 STMicroelectronics *****END OF FILE****/

