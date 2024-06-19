#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "DAC_files/DAC_micro_hot_plate.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"
#include "source/ADC_files/adc_16_strip.h"
#include "source/MUX_files/gpio_for_mux.h"
#include "source/DAC_files/DAC_excitation_voltage.h"
#include "source/Modbus_files/modbus.h"


// Handler for the SysTick interupt
void SysTick_Handler(void) {
    // Increment the timestamp
    g_timestamp_ms++;
}

// Function to parse the input string and update parameters
int updateParameters(char* input) {
    char* token;

    // Parse waveType
    token = strtok(input, ",");
    if (token != NULL) {
        waveType = atoi(token);
        PRINTF("waveType: %d\n", waveType);
    } else {
        PRINTF("Error parsing waveType\n");
        return -1;
    }

    // Parse dutyCycle
    token = strtok(NULL, ",");
    if (token != NULL) {
        dutyCycle = atoi(token);
        PRINTF("dutyCycle: %d\n", dutyCycle);
    } else {
        PRINTF("Error parsing dutyCycle\n");
        return -1;
    }

    // Parse desiredFrequency
    token = strtok(NULL, ",");
    if (token != NULL) {
        desiredFrequency = atof(token);
        PRINTF("desiredFrequency: %f\n", desiredFrequency);
    } else {
        PRINTF("Error parsing desiredFrequency\n");
        return -1;
    }

    // Parse desiredAmplitude
    token = strtok(NULL, ",");
    if (token != NULL) {
        desiredAmplitude = atof(token);
        PRINTF("desiredAmplitude: %f\n", desiredAmplitude);
    } else {
        PRINTF("Error parsing desiredAmplitude\n");
        return -1;
    }

    // Parse excitationVoltagePerStrip
    token = strtok(NULL, ",");
    if (token != NULL) {
        g_excitation_voltage_per_resistor = atof(token);
        PRINTF("excitationVoltagePerStrip: %f\n", g_excitation_voltage_per_resistor);
    } else {
        PRINTF("Error parsing excitationVoltagePerStrip\n");
        return -1;
    }

    // Parse mux_freq
    token = strtok(NULL, ",");
    if (token != NULL) {
        MUX_freq = atoi(token);
        PRINTF("mux_freq: %d\n", MUX_freq);
    } else {
        PRINTF("Error parsing mux_freq\n");
        return -1;
    }

    // Parse active strips, 1 mapped to true and 0 mapped to false. The order is from strip 1 to strip 8
    token = strtok(NULL, ",");
    if (token != NULL) {
        for (int i = 0; i < MAX_STRIP_COUNT; i++) {
            active_strips[i] = (token[i] == '1') ? true : false;
            PRINTF("active_strips[%d]: %d\n", i, active_strips[i]);
        }
    } else {
        PRINTF("Error parsing active_strips\n");
        return -1;
    }

    // Caluclate the number of strip based on the active_strips array and store it in g_strip_count
    for (int i = 0; i < MAX_STRIP_COUNT; i++) {
        if (active_strips[i]) {
            g_strip_count++;
        }
    }

    return 0; // Success
}

void calculate_resistance_array(void) {
    // Calculate the resistance array, loop through the active strips array and calculate the resistance of active strips
    // Leave the false entries at 0. Also add the timestamp from the V_sens_strip_values array
    for (int i = 0; i < MAX_STRIP_COUNT; i++) {
        if (active_strips[i]) {
            // Fill in the timestamp in the resistance array
            resistance_array[i][1] = V_sens_strip_values[i][1];
            
            // Calculate the resistance and round to the nearest integer
            // 0.5 is added to account for truncation when casting to int

            resistance_array[i][0] = (int)(((float)V_sens_strip_values[i][0] / (float)current_adc_result) * 120 * 75 + 0.5);
        }
    }
}



int initialize_dac_hot_plate(void) {
    // Initialize the DAC
    Dac_and_other_setup();

    // Proceed with DAC setup and wave generation only after receiving parameters
    g_Dac14ValueArraySize = (uint32_t)(SAMPLE_RATE / desiredFrequency);
    if (g_Dac14ValueArraySize > MAX_ARRAY_SIZE) {
        g_Dac14ValueArraySize = MAX_ARRAY_SIZE;
    }

    if (desiredFrequency < LOWEST_FREQ_POSSIBLE) {
        PRINTF("Desired frequency is too low, please enter a frequency above 0.002Hz\n");
        return -1;
    }

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
    }

    // Initialize the timer
    SetupTimer();

    return 0; // Success
}

int initialize_voltage_adc(void) {
    // Initialize the ADC
    ADC_Voltage_Initialize();

    return 0; // Success
}

int initialize_current_adc(void) {
    // Initialize the ADC
    ADC_Current_Initialize();

    ADC_Voltage_timer_setup(); /*We use the same timer setup for both ADCs*/

    return 0; // Success
}

int initialize_excitation_dac(void) {
    // Initialize the MUX
    DAC_excitation_V_Init();

    SetExcitationVoltage();

    return 0; // Success
}

int initialize_modbus(void) {
    /* Initialize LPUART2 for Modbus*/
    Modbus_init_UARTs();
    return 0; // Success
}

int main(void) {
    char buffer[256] = {0};
    int index = 0;
    char ch;
    int paramsReceived = 0;  
  
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // enable interupt and set priority for systick interupt
    NVIC_SetPriority(SysTick_IRQn, 1U);

    PRINTF("Waiting for parameters...\n");

    while (!paramsReceived) {
        ch = GETCHAR();  // Wait and read a character from UART
        buffer[index++] = ch;  // Store the received character in the buffer
        if (ch == '\n' || ch == '\r') {
            buffer[index] = '\0';  // Null-terminate the string
            PRINTF("Received string: %s\n", buffer);  // Print the received string for debugging
            if (updateParameters(buffer) == 0) {
                PRINTF("Parameters received and set.\n");
                paramsReceived = 1;
            } else {
                PRINTF("Error receiving parameters. Please resend.\n");
            }
            index = 0;
            buffer[0] = '\0';
        }
    }


    // Initialize the DAC and timer
    if (initialize_dac_hot_plate() != 0) {
    PRINTF("Failed to initialize microhotplate DAC and hot plate\n");
    // Handle error
    }

    // Initialize the ADC and timer
    if (initialize_voltage_adc() != 0) {
    PRINTF("Failed to initialize ADC and timer\n");
    // Handle error
    }

    // Initialize the ADC and timer
    if (initialize_current_adc() != 0) {
    PRINTF("Failed to initialize ADC and timer\n");
    // Handle error
    }

    // Initialize the excitation DAC
    if (initialize_excitation_dac() != 0) {
    PRINTF("Failed to initialize excitation DAC\n");
    // Handle error
    }

    /* Initialize LPUART2 for Modbus*/
    if (initialize_modbus() != 0) {
    PRINTF("Failed to initialize LPUART2 for Modbus\n");
    // Handle error
    }

    // Configure Systick for 1ms for the timestamps
    SysTick_Config(SystemCoreClock / 1000U);

    while (1) {
        /* Handle incoming Modbus frames*/
        HandleModbusFrame(MODBUS_LPUART);

        // Check if the array is filled and if so calculate the resistance array
        // if (V_sens_strip_values_ready && I_sens_strip_values_ready) {
        //     // Calculate the resistance array
        //     calculate_resistance_array();
        //     // Print the resistance array
        //     for (int i = 0; i < 8; i++) {
        //         PRINTF("Resistance of strip %d: %d, Time: %d\n", i + 1, resistance_array[i][0], resistance_array[i][1]);
        //     }
        //     // Reset the flags
        //     V_sens_strip_values_ready = false;
        //     I_sens_strip_values_ready = false;
        // }

        // // Check if the voltage array is ready and print it if its ready
        // if (V_sens_strip_values_ready) {
        //     for (int i = 0; i < 8; i++) {
        //         PRINTF("Voltage of strip %d: %d, Time: %d\n", i + 1, V_sens_strip_values[i][0], V_sens_strip_values[i][1]);
        //     }
        //     PRINTF("curent value: %d\n", current_adc_result);
        //     V_sens_strip_values_ready = false;
        // }

        // Print the current value

    }
}   


/*Priority list:
    1. - System tick
    2. - CTIMER2 for ADCs
    3. - ADC_Current and ADC_Voltage
    4. - CTIMER4 for DAC_microhotplate (DAC excitation doesn't need it)*/