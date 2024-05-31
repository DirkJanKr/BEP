#include <stdio.h>
#include <string.h>
#include "global.h"
#include "DAC_files/DAC_micro_hot_plate.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"
#include "source/ADC_files/adc_16_strip.h"

// Handler for the SysTick interupt
void SysTick_Handler(void) {
    // Increment the timestamp
    g_timestamp_ms++;
}

int main(void) {
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    Dac_and_other_setup();
    ADC_Initialize();

    // Proceed with DAC setup and wave generation only after receiving parameters
    g_Dac14ValueArraySize = (uint32_t)(SAMPLE_RATE / desiredFrequency);
    // if (g_Dac14ValueArraySize > MAX_ARRAY_SIZE) {
    //     g_Dac14ValueArraySize = MAX_ARRAY_SIZE;
    // }

    // Check if the desired frequency is within the range of the DACs capabilities
    // Chnage the lowest frequency possible in print statement if requirements change or use macro
    if (desiredFrequency < LOWEST_FREQ_POSSIBLE) {
        PRINTF("Desired frequency is too high, please enter a frequency above 0.002Hz\n");
        return -1;
    }

    // Depending on the required wave type, which the user will select, generate the wave
    switch (waveType) {
        case 0:
            generateSineWave(g_Dac14Values, g_Dac14ValueArraySize);
            break;
        case 1:
            generateSquareWave(g_Dac14Values, g_Dac14ValueArraySize);
            break;
        case 2:
            generateTriangleWave(g_Dac14Values, g_Dac14ValueArraySize);
            break;
        default:
            PRINTF("Invalid wave type\n");
            return -1;
            break;
    }

    // Start the timer to update the DAC
    SetupTimer();

    // Start the ADC timer
    float mux_freq = 16.0f;
    ADC_timer_setup(mux_freq);

    // Configure Systick for 1ms for the timestamps
    SysTick_Config(SystemCoreClock / 1000U);

    // enable interupt and set priority for systick interupt
    NVIC_SetPriority(SysTick_IRQn, 1U);

    while (1) {
        // Check if the array is filled
        if (g_strip_values_ready) {
            // Print the array with the value and time stamp for each strip
            for (uint8_t i = 0; i < g_strip_count; i++)
            {
                PRINTF("Strip %d: ADC value: %d, Time: %d\n", (i+1), g_strip_values[i][0], g_strip_values[i][1]);
            }

            // Reset the flag
            g_strip_values_ready = false;
        }
    }
}

