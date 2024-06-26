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
#include "source/drivers/fsl_ctimer.h"
#include "source/drivers/fsl_lpadc.h"
#include "source/drivers/fsl_dac.h"
#include "source/drivers/fsl_dac14.h"
#include "source/Modbus_files/modbus.h"
#include "source/PowerMUX_files/powermux.h"

// prototypes
int initialize_powermux(void);
int initialize_dac_hot_plate(void);
int initialize_mux(void);
int initialize_voltage_adc(void);
int initialize_current_adc(void);
int initialize_excitation_dac(void);
int initialize_Modbus(void);
int TimestampInit(void);


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

    // Caluclate the number of strip based on the active_strips array and store it in g_strip_count but first set it to 0
    g_strip_count = 0;
    for (int i = 0; i < MAX_STRIP_COUNT; i++) {
        if (active_strips[i]) {
            g_strip_count++;
        }
    }

    return 0; // Success
}

int InitializeAllPeripherals(){

    // Initialize the power mux
    if (initialize_powermux() != 0) {
        PRINTF("Failed to initialize power mux\n");
        return -1;
    }

    // Initialize Hot plate dac 
    if (initialize_dac_hot_plate() != 0) {
        PRINTF("Failed to initialize microhotplate DAC and hot plate\n");
        return -1;
    }

    // Initialize the MUX
    if (initialize_mux() != 0) {
        PRINTF("Failed to initialize MUX\n");
        return -1;
    }

    // Initialize the voltage ADC and timer
    if (initialize_voltage_adc() != 0) {
        PRINTF("Failed to initialize ADC and timer\n");
        return -1;
    }

    // Initialize the current
    if (initialize_current_adc() != 0) {
        PRINTF("Failed to initialize ADC and timer\n");
        return -1;
    }

    // Initialize the excitation DAC
    if (initialize_excitation_dac() != 0) {
        PRINTF("Failed to initialize excitation DAC\n");
        return -1;
    }

    // Initialize the timestamp timer
    if (TimestampInit() != 0) {
        PRINTF("Failed to initialize timestamp timer\n");
        return -1;
    }


    // Initialze resistance array to 0
    for (int i = 0; i < 8; i++) {
        resistance_array[i][0] = 0;
        resistance_array[i][1] = 0;
    }

    return 0; // Success
    
}

int initialize_powermux(void) {
    // Initialize the power mux
    InitPowerMux();

    return 0; // Success
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

int initialize_mux(void) {
    // Initialize the MUX
    InitMuxGpio();

    EnableMux();

    /*Select first active MUX channel*/
    SelectMuxChannel(FirstActiveStrip());

    // fill the array with active indices
    ConvertActiveStrips();

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

int TimestampInit(void) {
    // Reset the timestamp if a new mearement has started
    g_timestamp_ms = 0;
    // Configure Systick for 1ms for the timestamps
    SysTick_Config(SystemCoreClock / 1000U);

    // enable interupt and set priority for systick interupt
    NVIC_SetPriority(SysTick_IRQn, 1U);

    return 0; // Success
}

int initialize_Modbus(void) {
    // Initialize the MUX
    Modbus_init_UARTs();
    // holdingRegisters[7] = {0, 50, 50, 21000, 1000, 8, 8};
    // uint32_t waveType = 0;  // 0: sine, 1: square, 2: triangle (or sawtooth depending on the duty cycle)
    // uint32_t dutyCycle = 50;    // in percentage
    // float desiredFrequency = 0.05;  // in Hz
    // uint32_t desiredAmplitude = 21;  // in volts
    // float g_excitation_voltage_per_resistor = 1.0; // in volts;
    // uint16_t MUX_freq = 8;
    // bool active_strips[8] = {false, false, false, false, false, false, false, false};

    // InitializeAllPeripherals();

    return 0; // Success
}

int ResetAllPeripherals() {
    // Deinitialize timer for the micro hotplate DAC
    CTIMER_Deinit(CTIMER4);

    //Deinitialize Dac for the micro hotplate
    DAC14_Deinit(DEMO_DAC14_BASEADDR);

    // Deinitialize the MUX
    DisableMux();

    // Deinitialize the voltage ADC timer
    CTIMER_Deinit(CTIMER2);

    // Deinitialize the voltage ADC
    LPADC_Deinit(ADC0);

    // Deinitialize the current ADC
    LPADC_Deinit(ADC1);

    // Deinitialize the excitation DAC
    DAC_Deinit(DAC0);

    // Deinitialize the timestamp timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // Disable SysTick Timer

    return 0; // Success
}

int main(void) {
    char buffer[256] = {0};
    int index = 0;
    char ch;
    bool Initializationsuccess = false;

  
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Read the power source and setting the ModbusFlag
    readPowerSource();

    // enable interupt and set priority for systick interupt
    NVIC_SetPriority(SysTick_IRQn, 1U);

    PRINTF("Waiting for parameters...\n");

    if (ModbusFlag){
        initialize_Modbus();
        while(1){
            HandleModbusFrame(MODBUS_LPUART);
            PRINTF("--------------CHECK 3-------------\n");
            if(ModbusParamChanged){
                PRINTF("Parameters updated\n");
                InitializeAllPeripherals();
                ModbusParamChanged = false;
            }
        }
    }
    else{
        while(1){
            ch = GETCHAR();  // Wait and read a character from UART
            buffer[index++] = ch;  // Store the received character in the buffer
            if (ch == '\n' || ch == '\r') {
                buffer[index] = '\0';  // Null-terminate the string
                PRINTF("Received string: %s\n", buffer);  // Print the received string for debugging

                // reset active strips
                for (int i = 0; i < 8; i++) {
                    active_strips[i] = false;
                }
                if (Initializationsuccess == true) {
                    ResetAllPeripherals();
                    PRINTF("Reset all peripherals\n");
                }
                // Update parameters and initialize all peripherals
                if (updateParameters(buffer) == 0) {
                    if (InitializeAllPeripherals() != 0) {
                        PRINTF("Failed to initialize all peripherals\n");
                        Initializationsuccess = true;
                        return -1;
                    }
                    PRINTF("Parameters received and set.\n");

                } 
                else {
                    PRINTF("Error receiving parameters. Please resend.\n");
                }
                index = 0;
                buffer[0] = '\0';
            }
        }
    }
}   

// ch = GETCHAR();  // Wait and read a character from UART
//         buffer[index++] = ch;  // Store the received character in the buffer
//         if (ch == '\n' || ch == '\r') {
//             buffer[index] = '\0';  // Null-terminate the string
//             PRINTF("Received string: %s\n", buffer);  // Print the received string for debugging
//             // Update parameters and initialize all peripherals
//             if (updateParameters(buffer) == 0) {
//                 if (InitializeAllPeripherals() != 0) {
//                     PRINTF("Failed to initialize all peripherals\n");
//                     return -1;
//                 }
//                 PRINTF("Parameters received and set.\n");

//             } else {
//                 PRINTF("Error receiving parameters. Please resend.\n");
//             }
//             index = 0;
//             buffer[0] = '\0';
//             }


/*Priority list:
    1. - System tick
    2. - CTIMER2 for ADCs
    3. - ADC_Current and ADC_Voltage
    4. - CTIMER4 for DAC_microhotplate (DAC excitation doesn't need it)*/