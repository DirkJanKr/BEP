#include "source/global.h"
#include "DAC_micro_hot_plate.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "fsl_dac14.h"
#include "source/drivers/pin_mux.h"
#include "source/drivers/fsl_ctimer.h"
#include <math.h>

/*!
* @brief Scale the DAC values to the desired amplitude and opamp gain.
*/
void scaleDacValues(uint32_t *values, uint32_t size) {
    float maxDacValue = (desiredAmplitude / OPAMP_GAIN) * (16383.0 / 3.3);
    for (uint32_t i = 0; i < size; i++) {
        values[i] = (uint32_t)((float)values[i] / 16383.0 * maxDacValue);
    }
}

/*!
* @brief Generate a sine wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
*/
void generateSineWave(uint32_t *values, uint32_t size) {
    float deltaAngle = (2.0f * M_PI * desiredFrequency) / SAMPLE_RATE;
    for (uint32_t i = 0; i < size; i++) {
        float angle = i * deltaAngle;
        values[i] = (uint32_t)((sin(angle - M_PI_2) + 1.0) * 8191.5);  // Use sine function with phase shift
    }
    scaleDacValues(values, size);
}



/*!
* @brief Generate a square wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
*/
void generateSquareWave(uint32_t *values, uint32_t size) {
    if (dutyCycle < 0 || dutyCycle > 100) {
        PRINTF("Invalid duty cycle\n");
        return;
    }
    uint32_t onSize = (size * dutyCycle) / 100;
    for (uint32_t i = 0; i < onSize; i++) {
        values[i] = 16383;
    }
    for (uint32_t i = onSize; i < size; i++) {
        values[i] = 0;
    }
    scaleDacValues(values, size);
}

/*!
* @brief Generate a triangle wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
*/
void generateTriangleWave(uint32_t *values, uint32_t size) {
    // check if the duty cycle is valid
    if (dutyCycle < 0 || dutyCycle > 100) {
        PRINTF("Invalid duty cycle\n");
        return;
    }
    uint32_t onSize = (size * dutyCycle) / 100;
    float slope = 16383.0 / onSize;
    for (uint32_t i = 0; i < onSize; i++) {
        values[i] = i * slope;
    }
    for (uint32_t i = onSize; i < size; i++) {
        values[i] = 16383 - (i - onSize) * slope;
    }
    // // Print the values for debugging
    // for (uint32_t x = 0; x < size; x++) {
    //     PRINTF("Value: %d\n", values[x]);
    // }
    scaleDacValues(values, size);
    
}

/*!
* @brief Update the DAC output with the next value in the buffer.
*/
void dac_update_callback(uint32_t flags) {
    PRINTF("DAC value: %d\n", g_Dac14Values[g_Dac14InputIndex]);
    DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
    g_Dac14InputIndex = (g_Dac14InputIndex + 1) % g_Dac14ValueArraySize;
    DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
}

/*!
* @brief Setup the timer to trigger the DAC update callback at the desired sample rate.
*/
void SetupTimer() {
    // Enable the clock for the CTIMER
    CLOCK_SetClkDiv(kCLOCK_DivCtimer4Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CTIMER4); // Attach FRO 12MHz to CTIMER4.

    ctimer_config_t config;

    // Initialize the CTIMER
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER4, &config);

    // 12MHz is the clock frequency of the CTIMER
    uint32_t matchValue = 12000000 / SAMPLE_RATE;

    // Configure the match value
    ctimer_match_config_t matchConfig = {
        .enableCounterReset = true,
        .enableCounterStop = false,
        .matchValue = matchValue,
        .outControl = kCTIMER_Output_NoAction,
        .outPinInitState = false,
        .enableInterrupt = true,
    };

    // Use NVIC to enable the interrupt and set the priority
    EnableIRQ(CTIMER4_IRQn);
    NVIC_SetPriority(CTIMER4_IRQn, 2);

    // Register the callback function to be called when the match value is reached
    CTIMER_RegisterCallBack(CTIMER4, (ctimer_callback_t[]){(ctimer_callback_t)dac_update_callback}, kCTIMER_SingleCallback);
    CTIMER_SetupMatch(CTIMER4, kCTIMER_Match_0, &matchConfig);
    CTIMER_StartTimer(CTIMER4);
    PRINTF("CTIMER4 started with match value: %d\n", matchValue);
}

void CTIMER4_IRQHandler(void) {
    // Clear the interrupt flag
    CTIMER_ClearStatusFlags(CTIMER4, kCTIMER_Match0Flag);
    // Call the callback function
    dac_update_callback(0);
}

/*!
* @brief Setup the DAC and other peripherals.
*/
void Dac_and_other_setup() {
    
    // Enable the clock for the DAC
    CLOCK_SetClkDiv(kCLOCK_DivDac2Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_DAC2); // Attach FRO_HF to DAC2.

    // Configure the DAC
    dac14_config_t dac14ConfigStruct;
    DAC14_GetDefaultConfig(&dac14ConfigStruct);
    dac14ConfigStruct.TriggerSource = kDAC14_SoftwareTriggerSource;
    dac14ConfigStruct.WorkMode = kDAC14_FIFOWorkMode;
    dac14ConfigStruct.enableOpampBuffer = true;
    dac14ConfigStruct.enableDAC = true;
    DAC14_Init(DEMO_DAC14_BASEADDR, &dac14ConfigStruct);

    // Enable the analog module
    SPC0->ACTIVE_CFG1 |= 0x41;
}
