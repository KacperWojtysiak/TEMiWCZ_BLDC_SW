/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPD_H
#define __SPD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "main.h"
#include "ADC.h"
#include "g4xx_bemf_ADC_fdbk.h"
#include "bus_voltage_sensor.h"
#include <stdio.h>

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
typedef struct {
    int16_t speedRPM;      
} SPD_t;

extern SPD_t spd;
extern Bemf_ADC_Handle_t bemfHandle;
extern BusVoltageSensor_Handle_t BusVoltageHandle;
extern const Bemf_ADC_Params_t bemfAdcParams;
extern PWMC_Handle_t pwmHandle;

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
void InitBEMF(void);
void InitPWM(void);
void SPD_UpdateSpeedFromBEMF(void);

#ifdef __cplusplus
}
#endif

#endif
