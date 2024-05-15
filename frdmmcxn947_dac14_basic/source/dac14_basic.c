/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dac14.h"
#include <math.h>
#include "fsl_ctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SAMPLE_RATE 5000 // DAC sample rate
#define DEMO_DAC14_BASEADDR         DAC2
#define DEMO_DAC14_IRQ_ID           DAC2_IRQn
#define DEMO_DAC14_IRQ_HANDLER_FUNC DAC2_IRQHandler
#define DEMO_DAC14_VALUE_ARRAY_SIZE 1000 // Size of the DAC buffer
#define MAX_FREQUENCY 10  // Maximum frequency
#define MIN_FREQUENCY 0.1 // Minimum frequency
#define DESIRED_FREQUENCY 5.0 // Desired frequency

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void generateSineWave(uint32_t *values, uint32_t size, float frequency);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex    = 0U;
volatile uint32_t g_Dac14OutputIndex   = 0U;
volatile uint32_t g_Dac14InterruptDone = false;
uint32_t g_Dac14Values[DEMO_DAC14_VALUE_ARRAY_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
void dac_update_callback(uint32_t flags)
{
    // Update DAC with the next value from the sine wave array
    DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
    g_Dac14InputIndex = (g_Dac14InputIndex + 1) % DEMO_DAC14_VALUE_ARRAY_SIZE;
    DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
}

void SetupTimer(void) {
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);

    // Timer initialization
    CTIMER_Init(CTIMER0, &config);

    // Match configuration setup
    ctimer_match_config_t matchConfig = {
        .enableCounterReset = true,
        .enableCounterStop = false,
        .matchValue = CLOCK_GetFreq(kCLOCK_CoreSysClk) / SAMPLE_RATE,
        .outControl = kCTIMER_Output_Toggle,
        .outPinInitState = false,
        .enableInterrupt = true,
    };

    // Register the callback function
    CTIMER_RegisterCallBack(CTIMER0, (ctimer_callback_t[]){dac_update_callback}, kCTIMER_SingleCallback);

    // Apply match configuration
    CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig);

    // Start the timer
    CTIMER_StartTimer(CTIMER0);
}


// void CTIMER0_IRQHandler(void) {
//     CTIMER_ClearStatusFlags(CTIMER0, kCTIMER_Match0Flag);
//     DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
//     g_Dac14InputIndex = (g_Dac14InputIndex + 1) % DEMO_DAC14_VALUE_ARRAY_SIZE;
//     DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
// }


/*!
* @brief Generate a sine wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
*/
void generateSineWave(uint32_t *values, uint32_t size, float frequency) {
    float deltaAngle = (2.0f * M_PI * frequency) / SAMPLE_RATE;
    for (uint32_t i = 0; i < size; i++) {
        float angle = i * deltaAngle;
        values[i] = (uint32_t)((sin(angle) + 1.0) * 8191.5); // Scale sine to DAC range
    }

    // Normalize the values to the range of 0 to 16383
    for (uint32_t i = 0; i < size; i++) {
        if (values[i] < 0) {
            values[i] = 0;
        } else if (values[i] > 16383) {
            values[i] = 16383;
        }
    }
}
    
    
/*!
 * @brief Main function
 */
int main(void)
{
    // Initialize all required board-specific configurations
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // DAC configuration
    dac14_config_t dac14ConfigStruct;
    DAC14_GetDefaultConfig(&dac14ConfigStruct);
    dac14ConfigStruct.TriggerSource = kDAC14_SoftwareTriggerSource;
    dac14ConfigStruct.WorkMode = kDAC14_FIFOWorkMode;
    dac14ConfigStruct.enableOpampBuffer = true;
    dac14ConfigStruct.enableDAC = true;
    DAC14_Init(DEMO_DAC14_BASEADDR, &dac14ConfigStruct);

    // enable analog module
    SPC0->ACTIVE_CFG1 |= 0x41;

    // Configure the clock specific to the CTIMER instance being used
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    // Calculate the number of points per cycle for the sine wave
    uint32_t points_per_cycle = SAMPLE_RATE / DESIRED_FREQUENCY;
    PRINTF("Points per cycle: %d\n", points_per_cycle);
    PRINTF("Size of DAC buffer: %d\n", DEMO_DAC14_VALUE_ARRAY_SIZE);

    // Generate the sine wave data that will be used for DAC output
    generateSineWave(g_Dac14Values, points_per_cycle, DESIRED_FREQUENCY);

    // Setup the timer, including configuring callbacks and starting the timer
    SetupTimer();
    

    PRINTF("Enter while loop\n");
    // Main loop does nothing, just waits forever
    while (1)
    {
        PRINTF("In while loop\n");
    }
}


// void DEMO_DAC14_IRQ_HANDLER_FUNC(void)
// {
//     uint32_t flags = DAC14_GetStatusFlags(DEMO_DAC14_BASEADDR);

//     if (0U != (kDAC14_FIFOEmptyFlag & flags))
//     {
//         DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
//         if (g_Dac14InputIndex >= (DEMO_DAC14_VALUE_ARRAY_SIZE - 1U))
//         {
//             g_Dac14InputIndex = 0U;
//         }
//         else
//         {
//             g_Dac14InputIndex++;
//         }

//         // Trigger the DAC to output its value
//         DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
//     }

//     SDK_ISR_EXIT_BARRIER;
// }
