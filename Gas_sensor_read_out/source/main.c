#include <stdio.h>
#include <string.h>
#include "global.h"
#include "DAC_files/DAC_micro_hot_plate.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"

#define MAX_COMMAND_LEN 50

// Function to parse and process commands
void process_command(char *command) {
    char cmd[MAX_COMMAND_LEN];
    int freq, amplitude, duty_cycle;

    sscanf(command, "%s %d %d %d", cmd, &freq, &amplitude, &duty_cycle);

    if (strcmp(cmd, "SET_PARAMS") == 0) {
        PRINTF("Received SET_PARAMS command:\n");
        PRINTF("Frequency: %d Hz\n", freq);
        PRINTF("Amplitude: %d V\n", amplitude);
        PRINTF("Duty Cycle: %d %%\n", duty_cycle);
        // Update global variables
        dutyCycle = duty_cycle;
        desiredFrequency = freq;
        desiredAmplitude = amplitude;
    } 
}

int main(void) {
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    Dac_and_other_setup();

    // Serial port configuration
    char command[MAX_COMMAND_LEN];
    PRINTF("Waiting for command from Python...\n");
    
    // Loop to wait for the command
    while (1) {
        // Receive command from Python
        fgets(command, MAX_COMMAND_LEN, stdin);
        // Remove newline character from the end of the string
        command[strcspn(command, "\n")] = '\0';
                
        // Process received command
        process_command(command);

        // Break out of the loop once command is received
        if (strcmp(command, "") != 0) {
            break;
        }
    }

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
