/* ------------------------------------- INCLUDES -------------------------------------*/
#include "SPD.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

SPD_t spd = {
    .speedRPM = 0
};

BusVoltageSensor_Handle_t BusVoltageHandle = {
    .SensorType        = VIRTUAL_SENSOR,  // nie używasz fizycznego czujnika napięcia
    .ConversionFactor  = 3300,            // 3300 mV = 100% — 1 jednostka = 1 mV
    .LatestConv        = 3300,            // aktualne napięcie magistrali (mV)
    .AvBusVoltage_d    = 3300,            // wartość uśredniona — używana przez BADC_SetSamplingPoint
    .FaultState        = MC_NO_ERROR      // brak błędów napięcia
};

static PWMC_Params_t pwmParams = {
    .TIMx = TIM1,
    .pwm_en_u_port = SENS_V_A_GPIO_Port,
    .pwm_en_u_pin = SENS_V_A_Pin,
    .pwm_en_v_port = SENS_V_B_GPIO_Port,
    .pwm_en_v_pin = SENS_V_B_Pin,
    .pwm_en_w_port = SENS_V_C_GPIO_Port,
    .pwm_en_w_pin = SENS_V_C_Pin
};

PWMC_Handle_t pwmHandle;// = {
//     .CntPh             = PWM_PULSE,     // np. 50% wypełnienia przy ARR = 600
//     .StartCntPh        = 300,     // początkowe wypełnienie (przy starcie)
//     .ADCTriggerCnt     = PWM_PULSE,     // moment wyzwolenia ADC (w cyklach PWM)
//     .PWMperiod         = PWM_PERIOD_CYCLES,     // okres PWM (ARR)
//     .AlignFlag         = 0,       // brak błędu wyrównania
//     .NextStep          = 0,       // używane w niektórych aplikacjach 6-step
//     .Step              = 0,
//     .LSModArray        = {0, 1, 0, 1, 0, 1}, // typowo: 0–LS, 1–HS
//     .hElAngle          = 0,
//     .OverCurrentFlag   = false,
//     .OverVoltageFlag   = false,
//     .BrakeActionLock   = false,
//     .driverProtectionFlag = false,
//     .TurnOnLowSidesAction = false,
//     .QuasiSynchDecay   = false,
//     .pParams_str       = &pwmParams,
//     .pCCER_cfg         = NULL,
//     .pCCMR1_cfg        = NULL,
//     .pCCMR2_cfg        = NULL,
//     .TimerCfg          = NULL,
//     .LowSideOutputs    = LS_PWM_TIMER  // zakładamy że sterujesz timerem PWM bez specjalnych LS driverów
// };

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
    .IsLoopClosed = true, // Rozruch, potem true
    .SpeedTimerState = COMMUTATION,
    .SpeedBufferSize = 8,
    .SatSpeed = 32000,
};

Bemf_Sensing_Params bemfAdcConcfig = {
    .AdcThresholdPwmPerc  = 15,   // 15% Vbus – dla wysokiego duty (ON-sensing)
    .AdcThresholdHighPerc = 65,   // 65% Vbus – dla detekcji opadającej
    .AdcThresholdLowPerc  = 35,   // 35% Vbus – dla detekcji rosnącej

    .Bus2ThresholdConvFactor = 3300,  // Vbus = 3300 mV = 100%
    .ThresholdCorrectFactor = 0,      // offset korekcyjny – na początek 0

    .SamplingPointOff = 300,    // połowa okresu PWM (ARR = 600) → środek OFF-time
    .SamplingPointOn = 450,     // np. 75% ARR → jeśli ON-sensing się włączy

    .AWDfiltering = 3           // detekcja ZC po 3 kolejnych przekroczeniach
};

Bemf_Demag_Params bemfDemagConfig = {
    .DemagMinimumSpeedUnit = 100,     // RPM, przy których stosujemy min. czas demag
    .DemagMinimumThreshold = 300,     // w taktach timera (np. 300 us)

    // Wzór: czas_demag = RevUpDemagSpeedConv / speed
    .RevUpDemagSpeedConv = 30000,     // jeśli speed = 100 RPM, to 30000 / 100 = 300
    .RunDemagSpeedConv   = 30000
};

uint16_t zcRising2CommDelay   = 256;  // ~180 deg × (128/90) ≈ 256 → (w Q9)
uint16_t zcFalling2CommDelay  = 256;

uint16_t onSensingEnableThres  = 420;  // np. 70% ARR = 600 → 0.7*600
uint16_t onSensingDisableThres = 180;  // np. 30% ARR

uint16_t computationDelay = 2;  // użyj poprzedniego tStep jako bazowego (stabilniejsze)

void InitPWM(void)
{
    PWMC_Init(&pwmHandle);
    PWMC_SetAlignFlag(&pwmHandle, 0);
    PWMC_TurnOnLowSides(&pwmHandle, 500);  // 500 ticków na załadowanie bootstrapów
    HAL_Delay(1000);  // odczekaj 1 ms

    PWMC_SwitchOnPWM(&pwmHandle);
}

void InitBEMF(void){
    // BADC_SetBemfSensorlessParam(&bemfHandle, &bemfAdcConcfig, &zcRising2CommDelay, &zcFalling2CommDelay, 
    //                             &bemfDemagConfig, &onSensingEnableThres, &onSensingDisableThres, &computationDelay);

    bemfHandle.TIMClockFreq = HAL_RCC_GetPCLK1Freq();
    BADC_Init(&bemfHandle);
    // BADC_Clear(&bemfHandle); 
    BADC_SetDirection(&bemfHandle, 1);
}

// === Przerwanie komutacyjne (lub wywoływane co parę ms) ===
void SPD_UpdateSpeedFromBEMF()
{
    if (BADC_CalcAvrgMecSpeedUnit(&bemfHandle, &spd.speedRPM)) {
        ITM_SendValue(1, (uint32_t)spd.speedRPM );
        // Send to USB CDC
        // char msg[32];
        // snprintf(msg, sizeof(msg), "Speed: %d RPM\r\n", speedRPM);
        // CDC_Transmit_FS((uint8_t*)msg, strlen(msg));
    }
}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
    if ( start ) return;
    uint8_t step = 0xFF;
    switch (mc.step)
    {
        case 0:
            if (hcomp->Instance == COMP4) step = STEP_1; // W
            break;
        case 1:
            if (hcomp->Instance == COMP2) step = STEP_2; // V
            break;
        case 2:
            if (hcomp->Instance == COMP1) step = STEP_3; // U
            break;
        case 3:
            if (hcomp->Instance == COMP4) step = STEP_4; // W
            break;
        case 4:
            if (hcomp->Instance == COMP2) step = STEP_5; // V
            break;
        case 5:
            if (hcomp->Instance == COMP1) step = STEP_6; // U
            break;
    }

    if (step != 0xFF)
    {
        BADC_IsZcDetected(&bemfHandle, step);
        mc.step = step;
        // ⚡ Wykonaj natychmiastową komutację
    }
}