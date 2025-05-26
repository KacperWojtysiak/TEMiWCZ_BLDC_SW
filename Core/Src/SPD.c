/* ------------------------------------- INCLUDES -------------------------------------*/
#include "SPD.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/
PWMC_Handle_t PWM_Handle;
BusVoltageSensor_Handle_t BusVoltageHandle;

const Bemf_ADC_Params_t bemfAdcParams = {
    .LfTim = TIM2,                     // Timer do pomiaru czasu (ZC → ZC)
    .LfTimerChannel = TIM_CHANNEL_1,  // dowolny kanał (jeśli wymagany przez MCSDK)
    .pAdc = { ADC1, ADC1, ADC1, NULL, NULL, NULL },
    .AdcChannel = {
        ADC_CHANNEL_2,  // PA1 → faza A
        ADC_CHANNEL_4,  // PA3 → faza B
        ADC_CHANNEL_15, // PC5 → faza C
        0, 0, 0
    },
    .gpio_divider_available = false,  // brak załączanego dzielnika
    .bemf_divider_port = NULL,
    .bemf_divider_pin = 0
};

Bemf_ADC_Handle_t bemfHandle = {
    .pParams_str = &bemfAdcParams,
    .SpeedSamplingFreqHz = 100,
    .IsLoopClosed = true,
    .SpeedTimerState = COMMUTATION,
    .SpeedBufferSize = 8,
    .SatSpeed = 32000,
};

SPD_t spd = {
    .speedRPM = 0
};

// === Przerwanie komutacyjne (lub wywoływane co parę ms) ===
void UpdateSpeedFromBEMF()
{
    if (BADC_CalcAvrgMecSpeedUnit(&bemfHandle, &spd.speedRPM)) {
        ITM_SendValue(1, (uint32_t)spd.speedRPM );
        // Send to USB CDC
        // char msg[32];
        // snprintf(msg, sizeof(msg), "Speed: %d RPM\r\n", speedRPM);
        // CDC_Transmit_FS((uint8_t*)msg, strlen(msg));
    }
}

// === Funkcja ustawiająca punkt próbkowania (tylko raz przy starcie lub zmianie kroku) ===
void ConfigureSampling(void)
{
    BADC_SetSamplingPoint(&bemfHandle, &PWM_Handle, &BusVoltageHandle);
}
