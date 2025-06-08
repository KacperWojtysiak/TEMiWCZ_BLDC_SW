
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_H__
#define __MC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "main.h"
#include "mc_type.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
typedef enum
{
  ICLWAIT = 12,
  IDLE = 0,
  ALIGNMENT = 2,
  CHARGE_BOOT_CAP = 16,
  OFFSET_CALIB = 17,
  START = 4,
  SWITCH_OVER = 19,
  RUN = 6,
  STOP = 8,
  FAULT_NOW = 10,
  FAULT_OVER = 11,
  WAIT_STOP_MOTOR = 20
} MCI_State_t;

typedef struct
{
 float voltage;
 float current;
 float frequency;
 float padding [1];
} __attribute__ ((packed)) ScaleParams_t;

typedef enum
{
  MCI_NO_COMMAND = 0,
  MCI_START,
  MCI_ACK_FAULTS,
  MCI_MEASURE_OFFSETS,
  MCI_ALIGN_ENCODER,
  MCI_STOP 
} MCI_DirectCommands_t;

typedef enum
{
  MCI_BUFFER_EMPTY,  
  MCI_COMMAND_NOT_ALREADY_EXECUTED, 
  MCI_COMMAND_EXECUTED_SUCCESSFULLY,
  MCI_COMMAND_EXECUTED_UNSUCCESSFULLY
} MCI_CommandState_t ;

typedef enum
{
  MCI_NOCOMMANDSYET,           
  MCI_CMD_EXECSPEEDRAMP,       
  MCI_CMD_EXECTORQUERAMP,      
  MCI_CMD_SETCURRENTREFERENCES,
  MCI_CMD_SETOPENLOOPCURRENT,   
  MCI_CMD_SETOPENLOOPVOLTAGE,   
} MCI_UserCommands_t;

typedef struct
{
    // SpeednTorqCtrl_Handle_t *pSTC;         /*!< Speed and torque controller object used by MCI.*/
    pFOCVars_t pFOCVars;                   /*!< Pointer to FOC vars used by MCI.*/
    // PWMC_Handle_t *pPWM;                   /*!< Pointer to PWM handle structure.*/
    // VirtualSpeedSensor_Handle_t *pVSS;
    MCI_UserCommands_t lastCommand;        /*!< Last command coming from the user.*/
    int16_t hFinalSpeed;                   /*!< Final speed of last ExecSpeedRamp command.*/
    int16_t hFinalTorque;                  /*!< Final torque of last ExecTorqueRamp command.*/
    qd_t Iqdref;                           /*!< Current component of last SetCurrentReferences command.*/
    ScaleParams_t *pScale;
    uint16_t hDurationms;                  /*!< Duration in ms of last ExecSpeedRamp or ExecTorqueRamp command.*/
    MCI_DirectCommands_t DirectCommand;
    MCI_State_t State;
    uint16_t CurrentFaults;
    uint16_t PastFaults;
    MCI_CommandState_t CommandState;        /*!< The status of the buffered command.*/
    MC_ControlMode_t LastModalitySetByUser; /*!< The last MC_ControlMode_t set by the user. */
} MCI_Handle_t;


/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void MC_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MC_H__ */

