#include <stdio.h>
#include <string.h>
#include "global.h"
#include "DAC_files/DAC_micro_hot_plate.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"


int main(void) {
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    Dac_and_other_setup();

    // Proceed with DAC setup and wave generation only after receiving parameters
    g_Dac14ValueArraySize = (uint32_t)(SAMPLE_RATE / desiredFrequency);
    if (g_Dac14ValueArraySize > MAX_ARRAY_SIZE) {
        g_Dac14ValueArraySize = MAX_ARRAY_SIZE;
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

    while (1) {
        // Main loop does nothing, just waits forever
    }
}
