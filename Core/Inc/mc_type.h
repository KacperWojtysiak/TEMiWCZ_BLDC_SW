/**
  ******************************************************************************
  * @file    mc_type.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   Motor Control SDK global types definitions
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
  * @ingroup MC_Type
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MC_TYPE_H
#define MC_TYPE_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "fixpmath.h"
#include "impedcorr.h"
#include "hso.h"
#include "polpulse.h"
#include "pidreg_speed.h"
#include "pidregdqx_current.h"
#ifndef PROFILER_DISABLE
#include "profiler_handle.h"
#endif /* PROFILER_DISABLE */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup MC_Type Motor Control types
  * @{
  */

#include <mc_stm_types.h>

/* char definition to match Misra Dir 4.6 typedefs that indicate size and
 * signedness should be used in place ofthe basic numerical types */
typedef int8_t          char_t;

#ifndef _MATH
typedef float           float_t;
#endif

/** @name Macros to use bit banding capability */
/** @{ */
#define BB_REG_BIT_SET(regAddr,bit_number) *(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000 )<<5)\
                                           + (bit_number <<2)) = (uint32_t)(0x1u)
#define BB_REG_BIT_CLR(regAddr,bit_number) (*(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000)<<5)\
                                           + (bit_number <<2)) = (uint32_t)(0x0u))
#define BB_REG_BIT_READ(regAddr,bit_number) (*(uint32_t *) (0x42000000+(((uint32_t)regAddr - 0x40000000)<<5)\
                                           + (bit_number <<2)) )
/** @} */

/** @brief Not initialized pointer */
#define MC_NULL    (void *)(0x0)

/** @name Motor identification macros */
/** @{ */
#define M1      (uint8_t)(0x0)  /*!< Motor 1.*/
#define M2      (uint8_t)(0x1)  /*!< Motor 2.*/
#define M_NONE  (uint8_t)(0xFF) /*!< None motor.*/
/** @} */

/**
 * @anchor fault_codes
 * @name Fault codes
 * The symbols below define the codes associated to the faults that the
 * Motor Control subsystem can raise.
 * @{ */
#define  MC_NO_ERROR     ((uint16_t)0x0000) /**< @brief No error. */
#define  MC_NO_FAULTS    ((uint16_t)0x0000) /**< @brief No error. */
#define  MC_DURATION     ((uint16_t)0x0001) /**< @brief Error: FOC rate to high. */
#define  MC_OVER_VOLT    ((uint16_t)0x0002) /**< @brief Error: Software over voltage. */
#define  MC_UNDER_VOLT   ((uint16_t)0x0004) /**< @brief Error: Software under voltage. */
#define  MC_OVER_TEMP    ((uint16_t)0x0008) /**< @brief Error: Software over temperature. */
#define  MC_START_UP     ((uint16_t)0x0010) /**< @brief Error: Startup failed. */
#define  MC_SPEED_FDBK   ((uint16_t)0x0020) /**< @brief Error: Speed feedback. */
#define  MC_OVER_CURR    ((uint16_t)0x0040) /**< @brief Error: Emergency input (Over current). */
#define  MC_SW_ERROR     ((uint16_t)0x0080) /**< @brief Software Error. */
#define  MC_SAMPLEFAULT (uint16_t)(0x0100u)    /**< @brief Error: Fewer than two phase current samples available */
#define  MC_OVERCURR_SW (uint16_t)(0x0200u)    /**< @brief Error: Software overcurrent tripped */
#define  MC_DP_FAULT     ((uint16_t)0x0400) /**< @brief Error Driver protection fault. */

/** @}*/

/** @name Dual motor Frequency comparison definition */
/** @{ */
#define SAME_FREQ   0U
#define HIGHER_FREQ 1U
#define LOWER_FREQ  2U

#define HIGHEST_FREQ 1U
#define LOWEST_FREQ  2U
/** @} */

/** @name Error codes */
/** @{ */
#define MC_SUCCESS                          ((uint32_t)(0u))    /**< Success. The function executed successfully. */
#define MC_WRONG_STATE_ERROR                ((uint32_t)(1u))    /**< The state machine of the motor is not in a suitable state. */
#define MC_NO_POLARIZATION_OFFSETS_ERROR    ((uint32_t)(2u))    /**< Polarization offsets are needed but missing */

/** @} */

/** @brief Return type of API functions that return a status */
typedef uint32_t MC_RetStatus_t;

/**
  * @brief Structure holding D & Q measures or references, in Per-Unit format
  *
  * This type can be used to store either current or voltages in Q2.30 fixed
  * point format. This format allows for Per-Unit data storage.
  */
typedef Vector_dq_t dq_fixp30_t;

/**
  * @brief Groups currents and voltages ADC offset measurements
  *
  * The Polarization offsets measurement procedure is an operation that needs to be
  * done at least once in the course of product life, prior to its first activation. It
  * performs a set of measurements that allow for the Motor Control Application to
  * work well.
  *
  * @see See MC_SetPolarizationOffsetsMotor1 for more details.
  */
typedef struct {
     Currents_Irst_t CurrentOffsets; /**< ADC offset for phase current measurements */
     Voltages_Urst_t VoltageOffsets; /**< ADC offset for phase voltage measurements */
} PolarizationOffsets_t;

/**
  * @brief Structure holding D & Q measures or references, in Natural format
  *
  * This type can be used to store either current, voltages or frequencies in
  * Q16.16 fixed point format. This format allows for storing values in natural
  * units such as Ampere, Volt or Hertz for instance. It can be used as long as
  * the data to be stored can fit within the ranges
  */
typedef struct
{
    fixp16_t D;
    fixp16_t Q;
} Vector_dq_fixp16_t, dq_fixp16_t;

/**
 * @brief Structure holding D & Q measures or references, in floats
 *
 * This type can be used to store either current, voltages or frequencies in
 * float. This format allows for storing values in natural
 * units such as Ampere, Volt or Hertz for instance.
 */
typedef struct
{
    float D;
    float Q;
} Vector_dq_float_t, dq_float_t;

/**
  * @brief Lists all possible polarization offsets measurement states
  *
  * The Polarization offsets measurements is an operation that needs to be done at
  * least once in the course of product life, prior to its first activation. It
  * performs a set of measurements that allow for the Motor Control Application to
  * work well.
  *
  * @see See MC_SetPolarizationOffsetsMotor1 for more details.
  *
  * When the application is switched on, the Polarization State is set to #NOT_DONE.
  */
typedef enum {
    NOT_DONE = 0,    /**< No polarization offset have been provided to or measured by
                       * the Motor Control application since the application was started */
    ONGOING = 1,     /**< The polarization offset measurement procedure is on-going. The Motor
                       *  cannot be controlled yet. */
    COMPLETED = 2    /**< The polarization offsets measurement procedure has completed or
                       *  polarization offsets have been provided with MC_SetPolarizationOffsetsMotor1(). */
} PolarizationState_t;

typedef struct _FOC_ActiveCurrentLimits_Bits_
{
    uint16_t	FOC_ACL_Accel_Busvolt:1;
    uint16_t	FOC_ACL_Regen_Busvolt:1;
    uint16_t	FOC_ACL_Accel_Abs:1;
    uint16_t	FOC_ACL_Regen_Abs:1;
    uint16_t	FOC_ACL_Forwards:1;
    uint16_t	FOC_ACL_Reverse:1;
    uint16_t	FOC_ACL_Motor:1;
    uint16_t	FOC_ACL_Powerstage:1;
    uint16_t	unused:8;
} FOC_ActiveCurrentLimits_Bits;

/**
  * @brief union type definition for u32 to Float conversion and vice versa
  */
//cstat -MISRAC2012-Rule-19.2
typedef union _FLOAT_U32_
{
  uint32_t  U32_Val;
  float   Float_Val;
} FloatToU32;
//cstat +MISRAC2012-Rule-19.2

/* FOC ZEST Control structure
 *
 * This structure is used to control ZEST while running.
 * This is required because the ZEST library is controlled using function calls, and the internal data
 * structure is not available to the user.
 * The user can write to this structure via communication protocols or using the debugger, and the change
 * will be passed to ZEST using function calls.
 */
typedef struct _FOC_ZESTControl_s_
{
    bool		enableInjection;		/* Allow ZEST to inject a current signal into the motor */
    bool		enableCorrection;		/* Allow ZEST to correct the motor estimated angle */
    bool		enableFlip;				/* Allow HSO/ZEST to correct inverted rotor lock */

    bool 		enableRun;
    bool		enableRunBackground;

    bool		enableControlUpdate;

    fixp30_t		injectFreq_kHz;			/* Injection frequency in kHz */
    fixp30_t		injectId_A_pu;			/* Injection d-axis current in per unit A */
    fixp30_t		injectIq_A_pu;			/* Injection q-axis current in per unit A */
    fixp24_t		feedbackGainD;			/* D-axis feedback */
    fixp24_t		feedbackGainQ;			/* Q-axis feedback */

    fixp30_t		CurrentLimitToSwitchZestConfig_pu;	/* Threshold to switch to alternative ZeST configuration */
    fixp30_t		injectFreq_Alt_kHz;									/* Injection frequency in kHz for alternative configuration */
    fixp24_t		feedbackGainD_Alt;									/* D-axis feedback for alternative configuration */
    fixp24_t		feedbackGainQ_Alt;									/* Q-axis feedback for alternative configuration */
} FOC_ZESTControl_s;

typedef struct _FOC_ZestFeedback_s_
{
	/* Members will be renamed later on (ToDo) */
	fixp_t		signalD;
	fixp_t		signalQ;
	float_t		L;
	float_t		R;
	fixp30_t		thresholdFreq_pu;
} FOC_ZestFeedback_s;

typedef struct _throttleParams_t_
{
  float offset;
  float gain;
  float speedMaxRPM; //THROTTLE_SPEED_MAX_RPM
  int8_t direction;
  uint8_t padding [3];
} throttleParams_t;

/**
  * @brief union type definition for u32 to Float conversion and vice versa
  */
typedef union _CONVERT_u_
{
    uint32_t	u32;
    float_t		flt;
} CONVERT_u;

/**
  * @brief Two components q, d type definition
  */
typedef struct
{
  int16_t q;
  int16_t d;
} qd_t;

/**
  * @brief Two components q, d in float type
  */

typedef struct
{
    float q;
    float d;
} qd_f_t;

/**
  * @brief Two components a,b type definition
  */
typedef struct
{
  int16_t a;
  int16_t b;
} ab_t;

/**
  * @brief Two components a,b in float type
  */
typedef struct
{
  float a;
  float b;
} ab_f_t;

/**
  * @brief Two components alpha, beta type definition
  */
typedef struct
{
  int16_t alpha;
  int16_t beta;
} alphabeta_t;

/* ACIM definitions start */
typedef struct
{
  float fS_Component1;
  float fS_Component2;
} Signal_Components;

/**
  * @brief  Two components type definition
  */
typedef struct
{
  int16_t qVec_Component1;
  int16_t qVec_Component2;
} Vector_s16_Components;
/* ACIM definitions end */

/**
  * @brief  SensorType_t type definition, it's used in BusVoltageSensor and TemperatureSensor component parameters
  *         structures to specify whether the sensor is real or emulated by SW.
  */
typedef enum
{
  REAL_SENSOR, VIRTUAL_SENSOR
} SensorType_t;

/**
  * @brief  DOutputState_t type definition, it's used by DOUT_SetOutputState method of DigitalOutput class to specify
  *         the required output state.
  */
typedef enum
{
  INACTIVE, ACTIVE
} DOutputState_t;

/**
  * @brief  DrivingMode_t type definition, it's used by Bemf_ADC class to specify the driving mode type.
  */
typedef enum
{
  VM,  /**< @brief Voltage mode. */
  CM   /**< @brief Current mode. */
} DrivingMode_t;

/**
  * @brief  Specifies the control modality of the motor.
  */
typedef enum
{
  MCM_OBSERVING_MODE = 0,       /**< @brief All transistors are opened in order to let the motor spin freely and
                                  *   observe Bemf thanks to phase voltage measurements.
                                  *
                                  * Only relevant when HSO is used.
                                  */
  MCM_OPEN_LOOP_VOLTAGE_MODE,   /**< @brief Open loop, duty cycle set as reference. */
  MCM_OPEN_LOOP_CURRENT_MODE,   /**< @brief Open loop, q & d currents set as reference. */
  MCM_SPEED_MODE,               /**< @brief Closed loop, Speed mode.*/
  MCM_TORQUE_MODE,              /**< @brief Closed loop, Torque mode.*/
  MCM_PROFILING_MODE,           /**< @brief FW is configured to execute the motor profiler feature.
                                  *
                                  * Only relevant when HSO is used.
                                  */
  MCM_SHORTED_MODE,             /**< @brief Low sides are turned on.
                                  *
                                  * Only relevant when HSO is used.
                                  */
  MCM_POSITION_MODE,            /**< @brief Closed loop, sensored position control mode. */
  MCM_MODE_NUM                  /**< @brief Number of modes in enum. */
} MC_ControlMode_t;

/**
  * @brief Structure type definition for feed-forward constants tuning.
  */
typedef struct
{
  int32_t wConst_1D;
  int32_t wConst_1Q;
  int32_t wConst_2;
} FF_TuningStruct_t;

/**
  * @brief  Current references source type, internal or external to FOCDriveClass.
  */
typedef enum
{
  INTERNAL, EXTERNAL
} CurrRefSource_t ;

/**
  * @brief  FOC variables structure
  */
typedef struct
{
    uint16_t hCodeError;                        /**< @brief error message */

      /* Objects */
    fixp24_t		      	Flux_Rated_VpHz;	      /**< @brief Rated flux
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    /* Configuration */
    PolarizationState_t		PolarizationState;    /**< @brief polarization offsets measurements state
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    MC_ControlMode_t		controlMode;            /**< @brief control mode currently used
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    MC_ControlMode_t		controlMode_prev;       /**< @brief control mode in use in the previous cycle
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    bool				flagStartWithCalibration;			  /**< @brief Initiates the start-up and the polarization
                                                  *         offsets measurement procedure if needed.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    float_t       Kt;                           /**< @brief Torque to current conversion factor
                                                  *
                                                  * In Ampere/Nm
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */

    /* Stator Resistance */
    float_t				Rs_Ohm;                       /**< @brief Current phase motor resistance.
                                                  *
                                                  *  Used by RS DC estimation. In Ohm.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    fixp20_t      Rs_Rated_Ohm;                 /**< @brief Rated phase motor resistance
                                                  *
                                                  *  In Ohm.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    /* Stator inductance */
    /* User communication values in fixp24_t, range < 128 H, resolution ~= 60 nano-Henry */
    fixp24_t			Ls_Rated_H;                   /**< @brief Rated phase motor inductance
                                                  *
                                                  * In Henry. This value is in Q24 format. This allows
                                                  * for a 0 to 128 H range with a ~60 nano-Henry resolution.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    fixp24_t			Ls_Active_H;                  /**< @brief Current phase motor inductance
                                                  *
                                                  * In Henry. This value is in Q24 format. This allows
                                                  * for a 0 to 128 H range with a ~60 nano-Henry resolution.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */

    /* Zest estimated Ls */
    fixp24_t			Ls_Est_H;				              /**< @brief Estimated phase motor inductance
                                                  *
                                                  * In Henry. This value is in Q24 format. This allows
                                                  * for a 0 to 128 H range with a ~60 nano-Henry resolution.
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */

    FIXP_scaled_t			Ls_Rated_pu_fps;		      /**< @brief Rated phase motor inductance, in "per unit"
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    FIXP_scaled_t			Ls_Active_pu_fps;		      /**< @brief Current phase motor inductance, in "per unit"
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    float         polePairs;                    /**< @brief Number of pole pairs of the motor
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */
    bool        flag_enableOffsetMeasurement;   /**< @brief Enables or disables the measurement of polarization offsets
                                                  *
                                                  * @note this field only exists if HSO is used.
                                                  */

} FOCVars_t, *pFOCVars_t;

/**
  * @brief  StartUp mode definition
  */

typedef enum
{
  POL_PULSE_ONLY = 0x0U,    /**< @brief Start with PolPulses only */
  RSDC_ONLY      = 0x1U     /**< @brief Start with RsDC only (with alignment) */
} StartMode_t, *pStartMode_t;

/**
  * @brief  6step variables structure.
  */
typedef struct
{
  uint16_t DutyCycleRef;        /**< @brief Reference speed. */
  uint16_t hCodeError;          /**< @brief error message. */
  CurrRefSource_t bDriveInput;  /**< @brief It specifies whether the current reference source must be
                                 *          #INTERNAL or #EXTERNAL. */
  int16_t qElAngle;
} SixStepVars_t, *pSixStepVars_t;

/**
  * @brief  Low side or enabling signal definition.
  */

typedef enum
{
  LS_DISABLED  = 0x0U,   /**< @brief Low side signals and enabling signals always off.
                                     It is equivalent to DISABLED. */
  LS_PWM_TIMER = 0x1U,   /**< @brief Low side PWM signals are generated by timer.
                                     It is equivalent to ENABLED. */
  ES_GPIO   = 0x2U       /**< @brief Enabling signals are managed by GPIOs (L6230 mode). */
} LowSideOutputsFunction_t;

/** @name UserInterface related exported definitions. */
/** @{ */
#define OPT_NONE    0x00 /**< @brief No UI option selected. */
#define OPT_COM     0x02 /**< @brief Bit field indicating that the UI uses serial communication. */
#define OPT_DAC     0x04 /**< @brief Bit field indicating that the UI uses real DAC. */
#define OPT_DACT    0x08 /**< @brief Bit field indicating that the UI uses RC Timer DAC. */
#define OPT_DACS    0x10 /**< @brief Bit field indicating that the UI uses SPI communication. */
#define OPT_DACF3   0x40 /**< @brief Bit field indicating that the UI uses DAC for STM32F3. */
#define OPT_DACF072 0x80 /**< @brief Bit field indicating that the UI uses DAC for STM32F072. */
/** @} */

#define MAIN_SCFG_POS (28)
#define AUX_SCFG_POS  (24)

#define MAIN_SCFG_VALUE(x) (((x)>>MAIN_SCFG_POS)& ( uint8_t )0x0F)
#define AUX_SCFG_VALUE(x)  (((x)>>AUX_SCFG_POS)& ( uint8_t )0x0F)

/** @name PFC related exported definitions */
/** @{ */

#define PFC_SWE             0x0001U /**< @brief PFC Software error. */
#define PFC_HW_PROT         0x0002U /**< @brief PFC hardware protection. */
#define PFC_SW_OVER_VOLT    0x0004U /**< @brief PFC software over voltage. */
#define PFC_SW_OVER_CURRENT 0x0008U /**< @brief PFC software over current. */
#define PFC_SW_MAINS_FREQ   0x0010U /**< @brief PFC mains frequency error. */
#define PFC_SW_MAIN_VOLT    0x0020U /**< @brief PFC mains voltage error. */
/** @} */

/** @name Definitions exported for the DAC channel used as reference for protection. */
/** @{ */
#define AO_DISABLED 0x00U /**< @brief Analog output disabled. */
#define AO_DEBUG    0x01U /**< @brief Analog output debug. */
#define VREF_OCPM1  0x02U /**< @brief Voltage reference for over current protection of motor 1. */
#define VREF_OCPM2  0x03U /**< @brief Voltage reference for over current protection of motor 2. */
#define VREF_OCPM12 0x04U /**< @brief Voltage reference for over current protection of both motors. */
#define VREF_OVPM12 0x05U /**< @brief Voltage reference for over voltage protection of both motors. */
/** @} */

/** @name ADC channel number definitions */
/** @{ */
#define MC_ADC_CHANNEL_0     0
#define MC_ADC_CHANNEL_1     1
#define MC_ADC_CHANNEL_2     2
#define MC_ADC_CHANNEL_3     3
#define MC_ADC_CHANNEL_4     4
#define MC_ADC_CHANNEL_5     5
#define MC_ADC_CHANNEL_6     6
#define MC_ADC_CHANNEL_7     7
#define MC_ADC_CHANNEL_8     8
#define MC_ADC_CHANNEL_9     9
#define MC_ADC_CHANNEL_10    10
#define MC_ADC_CHANNEL_11    11
#define MC_ADC_CHANNEL_12    12
#define MC_ADC_CHANNEL_13    13
#define MC_ADC_CHANNEL_14    14
#define MC_ADC_CHANNEL_15    15
#define MC_ADC_CHANNEL_16    16
#define MC_ADC_CHANNEL_17    17
#define MC_ADC_CHANNEL_18    18
#define MC_ADC_CHANNEL_19    19
#define MC_ADC_CHANNEL_20    20
#define MC_ADC_CHANNEL_21    21
#define MC_ADC_CHANNEL_22    22
/** @} */

/** @name Utility macros definitions */
/** @{ */
#define RPM2MEC01HZ(rpm) (int16_t)((int32_t)(rpm)/6)
#define MAX(a,b) (((a)>(b))?(a):(b))
/** @} */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* MC_TYPE_H */
/******************* (C) COPYRIGHT 2024 STMicroelectronics *****END OF FILE****/

