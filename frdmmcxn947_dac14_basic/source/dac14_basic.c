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
#define SAMPLE_RATE 5000 // DAC update sample rate in Hz
#define DESIRED_FREQUENCY 5.0 // Desired output frequency in Hz
#define DUTY_CYCLE 50 // Duty cycle of the square wave in percentage also used for triangle wave to determine the slope of the triangle wave
#define MAX_ARRAY_SIZE 1000 // Maximum size of the DAC buffer
#define DEMO_DAC14_BASEADDR DAC2

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void generateSineWave(uint32_t *values, uint32_t size);
void dac_update_callback(uint32_t flags);
void SetupTimer();
void generateSquareWave(uint32_t *values, uint32_t size);
void generateTriangleWave(uint32_t *values, uint32_t size);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex = 0U;
uint32_t g_Dac14Values[MAX_ARRAY_SIZE]; // Fixed-size array to hold the sine wave values
uint32_t g_Dac14ValueArraySize; // Actual size of the array used
uint32_t waveType = 2; // 0 = sine, 1 = square, 2 = triangle

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Generate a sine wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
 */
void generateSineWave(uint32_t *values, uint32_t size)
{
    float deltaAngle = (2.0f * M_PI * DESIRED_FREQUENCY) / SAMPLE_RATE;
    for (uint32_t i = 0; i < size; i++)
    {
        float angle = i * deltaAngle;
        values[i] = (uint32_t)((sin(angle) + 1.0) * 8191.5); // Scale sine to DAC range
    }
}

/*!
 * @brief Generate a square wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383. 
 * The duty cycle of the square wave is defined by the DUTY_CYCLE macro. And is taken into account when generating the square wave.
 */
void generateSquareWave(uint32_t *values, uint32_t size)
{
    // Check to see if the duty cycle is between 0 and 100 and if not quit the entire program and print an error message
    if (DUTY_CYCLE < 0 || DUTY_CYCLE > 100)
    {
        PRINTF("Invalid duty cycle\n");
        return;
    }
    uint32_t onSize = (size * DUTY_CYCLE) / 100;
    for (uint32_t i = 0; i < onSize; i++)
    {
        values[i] = 16383;
    }
    for (uint32_t i = onSize; i < size; i++)
    {
        values[i] = 0;
    }
}

/*!
 * @brief Generate a triangle wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
 * The triangle wave should have the desired frequency. Thus one period of the triangle wave starts at 0 and ends at 0.
 * When the duty cycle is set to 100 the triangle wave will be a sawtooth wave.
 */
void generateTriangleWave(uint32_t *values, uint32_t size)
{
    // Check to see if the duty cycle is between 0 and 100 and if not quit the entire program and print an error message
    if (DUTY_CYCLE < 0 || DUTY_CYCLE > 100)
    {
        PRINTF("Invalid duty cycle\n");
        return;
    }

    // Calculate the size of the first half of the triangle wave taking the DUTY_CYCLE into account
    uint32_t onSize = (size * DUTY_CYCLE) / 100;
    // Calculate the slope of the triangle wave
    float slope = 16383.0 / onSize;
    // Generate the first half of the triangle wave
    for (uint32_t i = 0; i < onSize; i++)
    {
        values[i] = i * slope;
    }
    // Generate the second half of the triangle wave
    for (uint32_t i = onSize; i < size; i++)
    {
        values[i] = 16383 - (i - onSize) * slope;
    }
}

/*!
 * @brief Timer callback function to update the DAC.
 */
void dac_update_callback(uint32_t flags)
{
    DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
    g_Dac14InputIndex = (g_Dac14InputIndex + 1) % g_Dac14ValueArraySize;
    DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
}

/*!
 * @brief Setup the timer to generate periodic interrupts.
 */
void SetupTimer()
{
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER4, &config);

     uint32_t matchValue = 48000000 / SAMPLE_RATE;

    ctimer_match_config_t matchConfig = {
        .enableCounterReset = true,
        .enableCounterStop = false,
        .matchValue = matchValue,
        .outControl = kCTIMER_Output_Toggle,
        .outPinInitState = false,
        .enableInterrupt = true,
    };

    CTIMER_RegisterCallBack(CTIMER4, (ctimer_callback_t[]){(ctimer_callback_t)dac_update_callback}, kCTIMER_SingleCallback);
    CTIMER_SetupMatch(CTIMER4, kCTIMER_Match_0, &matchConfig);
    CTIMER_StartTimer(CTIMER4);
    PRINTF("Timer started with match value: %d\n", matchValue);
}

/*!
 * @brief Main function
 */
int main(void)
{
    // Initialize board, clocks, and debug console
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Enable the clock for CTIMER4
    CLOCK_SetClkDiv(kCLOCK_DivCtimer4Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER4);  // Attach a suitable clock source to CTIMER4

    // Print clock frequency of the timer
    uint32_t timerClockFreq = CLOCK_GetFreq(kCLOCK_DivCtimer4Clk);
    PRINTF("CTIMER4 clock frequency: %d\n", timerClockFreq);

    // Configure clock for DAC2
    CLOCK_SetClkDiv(kCLOCK_DivDac2Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_DAC2);  // Attach a suitable clock source to DAC2

    // DAC configuration
    dac14_config_t dac14ConfigStruct;
    DAC14_GetDefaultConfig(&dac14ConfigStruct);
    dac14ConfigStruct.TriggerSource = kDAC14_SoftwareTriggerSource;
    dac14ConfigStruct.WorkMode = kDAC14_FIFOWorkMode;
    dac14ConfigStruct.enableOpampBuffer = true;
    dac14ConfigStruct.enableDAC = true;
    DAC14_Init(DEMO_DAC14_BASEADDR, &dac14ConfigStruct);

    // Enable analog module
    SPC0->ACTIVE_CFG1 |= 0x41;

    // Calculate the size of the DAC buffer based on the sample rate and desired frequency
    g_Dac14ValueArraySize = (uint32_t)(SAMPLE_RATE / DESIRED_FREQUENCY);
    if (g_Dac14ValueArraySize > MAX_ARRAY_SIZE) {
        g_Dac14ValueArraySize = MAX_ARRAY_SIZE;
    }

    PRINTF("DAC buffer size: %d\n", g_Dac14ValueArraySize);

    // Fill the buffer with the required type of wave
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

    // Setup the timer
    SetupTimer();

    // Main loop does nothing, just waits forever
    while (1)
    {
    }
}
