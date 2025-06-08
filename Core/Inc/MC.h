
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_H__
#define __MC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "main.h"

/* --------------------------------- PUBLIC VARIABLES ---------------------------------*/
typedef struct {
    uint8_t step;
    const uint16_t pwmDuty;
    volatile bool enabled;
} McScheduler_t;

extern McScheduler_t mc;

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MC_H__ */

