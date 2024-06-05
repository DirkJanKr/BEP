#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"
#include "source/drivers/clock_config.h"
#include "board.h"
#include "source/drivers/fsl_dac.h"
#include "source/global.h"
#include <stdio.h>
#include "DAC_excitation_voltage.h"
#include "source/drivers/fsl_dac.h"

#define DEMO_DAC_BASEADDR DAC0
#define DEMO_DAC_VREF     kDAC_ReferenceVoltageSourceAlt1
#define AMPLIFIER_GAIN    3.0  // Amplifier gain


void DAC_excitation_V_Init(void)
{
    dac_config_t dacConfigStruct;

    CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_DAC0);

    /* enable DAC0 and VREF */
    SPC0->ACTIVE_CFG1 |= 0x11;

    /* Configure the DAC. */
    DAC_GetDefaultConfig(&dacConfigStruct);
    dacConfigStruct.referenceVoltageSource = DEMO_DAC_VREF;
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable the logic and output. */

    DAC_SetData(DEMO_DAC_BASEADDR, 0); // Turn off DAC output
}

void SetExcitationVoltage(void)
{
    if (!g_excitation_voltage_on)
    {
        DAC_SetData(DEMO_DAC_BASEADDR, 0); // Turn off DAC output
        PRINTF("Excitation voltage turned off.\n");
        return;
    }

    if (g_excitation_voltage_per_resistor < 0.5 || g_excitation_voltage_per_resistor > 1.0)
    {
        PRINTF("Error: Excitation voltage per resistor must be between 0.5V and 1.0V.\n");
        return;
    }

    float total_voltage = g_excitation_voltage_per_resistor * g_resistor_formation;
    if (total_voltage > 8.0)
    {
        PRINTF("Error: Total excitation voltage must not exceed 8.0V.\n");
        return;
    }

    // Calculate the DAC value considering the amplifier gain
    float dac_output_voltage = total_voltage / AMPLIFIER_GAIN;
    uint32_t dacValue = (uint32_t)((dac_output_voltage / 3.3) * 4095); // Assuming 3.3V reference
    if (dacValue > 4095)
    {
        PRINTF("Error: DAC value out of range.\n");
        return;
    }

    DAC_SetData(DEMO_DAC_BASEADDR, dacValue);

    // Print values by converting floats to integer representation
    int total_voltage_int = (int)(total_voltage * 100); // Total voltage multiplied by 100
    int dac_output_voltage_int = (int)(dac_output_voltage * 100); // DAC output voltage multiplied by 100

    PRINTF("DAC output set to %d (corresponding to %d.%02dV, amplified to %d.%02dV).\n",
        dacValue,
        dac_output_voltage_int / 100, dac_output_voltage_int % 100,
        total_voltage_int / 100, total_voltage_int % 100);
}

/*To add later on in the main.c file*/

// /*!
//  * @brief Main function
//  */
// int main(void)
// {
//     dac_config_t dacConfigStruct;

//     /* attach FRO 12M to FLEXCOMM4 (debug console) */
//     CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
//     CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

//     /* attach FRO HF to DAC0 */
//     CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1u);
//     CLOCK_AttachClk(kFRO_HF_to_DAC0);

//     /* enable DAC0 and VREF */
//     SPC0->ACTIVE_CFG1 |= 0x11;

//     BOARD_InitPins();
//     BOARD_InitBootClocks();
//     BOARD_InitDebugConsole();

//     PRINTF("\nDAC Excitation Voltage Example.\n");

//     /* Configure the DAC. */
//     DAC_GetDefaultConfig(&dacConfigStruct);
//     dacConfigStruct.referenceVoltageSource = DEMO_DAC_VREF;
//     DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
//     DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable the logic and output. */

//     DAC_SetData(DEMO_DAC_BASEADDR, 0); // Turn off DAC output


//     while (1)
//     {
//         SetExcitationVoltage();
//     }
// }
