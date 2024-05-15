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
#define DEMO_DAC14_BASEADDR DAC2
#define DEMO_DAC14_VALUE_ARRAY_SIZE 1000 // Size of the DAC buffer
#define DESIRED_FREQUENCY 5.0 // Desired output frequency in Hz

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void generateSineWave(uint32_t *values, uint32_t size, float frequency);
void dac_update_callback(uint32_t flags);
void SetupTimer(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex = 0U;
uint32_t g_Dac14Values[DEMO_DAC14_VALUE_ARRAY_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Generate a sine wave in the DAC buffer. The values should be in the range of 0 to 16383 and never negative or greater than 16383.
 */
void generateSineWave(uint32_t *values, uint32_t size, float frequency)
{
    float deltaAngle = (2.0f * M_PI * frequency) / SAMPLE_RATE;
    for (uint32_t i = 0; i < size; i++)
    {
        float angle = i * deltaAngle;
        values[i] = (uint32_t)((sin(angle) + 1.0) * 8191.5); // Scale sine to DAC range
    }
}

/*!
 * @brief Timer callback function to update the DAC.
 */
void dac_update_callback(uint32_t flags)
{
    DAC14_SetData(DEMO_DAC14_BASEADDR, g_Dac14Values[g_Dac14InputIndex]);
    g_Dac14InputIndex = (g_Dac14InputIndex + 1) % DEMO_DAC14_VALUE_ARRAY_SIZE;
    DAC14_DoSoftwareTrigger(DEMO_DAC14_BASEADDR);
}

/*!
 * @brief Setup the timer to generate periodic interrupts.
 */
void SetupTimer(void)
{
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER4, &config);

    uint32_t timerClockFreq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
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
    PRINTF("Timer started with match value: %d\n", matchConfig.matchValue);
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
    PRINTF("CTIMER4 clock frequency: %d\n", CLOCK_GetFreq(kCLOCK_DivCtimer4Clk));

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

    // Generate the sine wave data that will be used for DAC output
    generateSineWave(g_Dac14Values, DEMO_DAC14_VALUE_ARRAY_SIZE, DESIRED_FREQUENCY);

    // Setup the timer
    SetupTimer();

    // Main loop does nothing, just waits forever
    while (1)
    {
    }
}
