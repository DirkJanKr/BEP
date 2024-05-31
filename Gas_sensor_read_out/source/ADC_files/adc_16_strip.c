#include "fsl_debug_console.h"
#include "source/drivers/pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "source/drivers/fsl_lpadc.h"
#include "fsl_common.h"
#include "source/drivers/fsl_vref.h"
#include "fsl_spc.h"
#include "source/drivers/fsl_ctimer.h"
#include "source/global.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SPC_BASE               SPC0
#define DEMO_LPADC_BASE             ADC0
#define DEMO_LPADC_IRQn             ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define DEMO_LPADC_USER_CHANNEL     2U
#define DEMO_LPADC_USER_CMDID       1U
#define DEMO_LPADC_VREF_SOURCE      kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION true
#define DEMO_LPADC_OFFSET_VALUE_A   0x10U
#define DEMO_LPADC_OFFSET_VALUE_B   0x10U
#define DEMO_LPADC_USE_HIGH_RESOLUTION   true

#define DEMO_VREF_BASE VREF0

// used when averaging ADC values
// #define AVERAGE_COUNT 100


/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t g_LpadcResultConfigStruct;
volatile uint16_t g_AdcValue = 0;

#if (defined(DEMO_LPADC_USE_HIGH_RESOLUTION) && DEMO_LPADC_USE_HIGH_RESOLUTION)
const uint32_t g_LpadcFullRange = 65536U;
const uint32_t g_LpadcResultShift = 0U;
#else
const uint32_t g_LpadcFullRange = 4096U;
const uint32_t g_LpadcResultShift = 3U;
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */

// Variable to keep track of the number of graphene strips to be read
static uint8_t current_strip = 0;
// Referernce voltage, I am not sure how accurate this 3.3V is and not sure how tocheck this.
float g_reference_voltage = 3.3;

// Used when averaging ADC values
// static uint16_t adc_value_buffer[AVERAGE_COUNT] = {0};
// static uint8_t adc_value_index = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * ADC Interrupt Handler
 * This function is called when the ADC conversion is complete.
 * It retrieves the conversion result and prints it.
 */
void ADC0_IRQHandler(void) {
    if (LPADC_GetConvResult(DEMO_LPADC_BASE, &g_LpadcResultConfigStruct, 0U)) {

        g_LpadcConversionCompletedFlag = true;
        g_AdcValue = (g_LpadcResultConfigStruct.convValue >> g_LpadcResultShift);

        // Commented code below can be used for averaging values

        // // Add the new ADC value to the buffer
        // adc_value_buffer[adc_value_index] = g_AdcValue;
        // adc_value_index = (adc_value_index + 1) % AVERAGE_COUNT;

        // // Compute the average of the buffer values
        // uint32_t sum = 0;
        // for (uint8_t i = 0; i < AVERAGE_COUNT; i++) {
        //     sum += adc_value_buffer[i];
        // }
        // uint16_t average_value = sum / AVERAGE_COUNT;

        // // Save the averaged ADC value and timestamp to the current strip
        // g_strip_values[current_strip][0] = average_value; // Save averaged ADC value
        // g_strip_values[current_strip][1] = g_timestamp_ms; // Save timestamp

        // Code below is without averaging, just the 1024 hardware averaging
        g_strip_values[current_strip][0] = g_AdcValue; // Save ADC value
        g_strip_values[current_strip][1] = g_timestamp_ms; // Save timestamp

        // Move to the next strip
        current_strip++;
        if (current_strip >= g_strip_count) {
            current_strip = 0; // Reset the strip index
            g_strip_values_ready = true; // Set the flag indicating the array is filled
        }
        // MUX channel selection here
    }
    SDK_ISR_EXIT_BARRIER;
}


/**
 * Initialize the ADC
 * This function sets up the ADC with the required configurations and calibrations.
 */
void ADC_Initialize(void) {
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;
    vref_config_t vrefConfig;

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

    SPC_EnableActiveModeAnalogModules(DEMO_SPC_BASE, kSPC_controlVref);

    VREF_GetDefaultConfig(&vrefConfig);
    vrefConfig.bufferMode = kVREF_ModeBandgapOnly;
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
#if defined(DEMO_LPADC_VREF_SOURCE)
    mLpadcConfigStruct.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
#endif /* DEMO_LPADC_VREF_SOURCE */
#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage1024;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
#if defined(DEMO_LPADC_DO_OFFSET_CALIBRATION) && DEMO_LPADC_DO_OFFSET_CALIBRATION
    LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
#else
    LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
#endif /* DEMO_LPADC_DO_OFFSET_CALIBRATION */
#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE
    LPADC_SetOffsetCalibrationMode(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_CALIBRATION_MODE);
    LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE */
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = DEMO_LPADC_USER_CHANNEL;
#if defined(DEMO_LPADC_USE_HIGH_RESOLUTION) && DEMO_LPADC_USE_HIGH_RESOLUTION
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct);

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFO0WatermarkInterruptEnable);
#else
    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFOWatermarkInterruptEnable);
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
    EnableIRQWithPriority(DEMO_LPADC_IRQn, 3); // Set ADC interrupt priority to 3

    PRINTF("ADC Full Range: %d\n", g_LpadcFullRange);
#if defined(FSL_FEATURE_LPADC_HAS_CMDL_CSCALE) && FSL_FEATURE_LPADC_HAS_CMDL_CSCALE
    if (kLPADC_SampleFullScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Full channel scale (Factor of 1).\n");
    }
    else if (kLPADC_SamplePartScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Divided input voltage signal. (Factor of 30/64).\n");
    }
#endif
}

/**
 * Setup the ADC timer
 * This function configures the timer to generate interrupts at the desired frequency.
 */
void ADC_timer_setup(float mux_frequency) {
    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CTIMER2); // Attach FRO 12MHz to CTIMER2.

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER2, &config);

    uint32_t matchValue = 12000000 / mux_frequency;

    ctimer_match_config_t matchConfig = {
        .enableCounterReset = true,
        .enableCounterStop = false,
        .matchValue = matchValue,
        .outControl = kCTIMER_Output_NoAction,
        .outPinInitState = false,
        .enableInterrupt = true,
    };

    // Use NVIC to enable the interrupt and set the priority
    EnableIRQ(CTIMER2_IRQn);
    NVIC_SetPriority(CTIMER2_IRQn, 2);

    CTIMER_SetupMatch(CTIMER2, kCTIMER_Match_0, &matchConfig);
    CTIMER_StartTimer(CTIMER2);
    PRINTF("CTIMER2 started with match value: %d\n", matchValue);
}

/**
 * Timer Interrupt Handler
 * This function is called when the timer match occurs.
 * It triggers an ADC conversion.
 */
void CTIMER2_IRQHandler(void) {
    // Clear the interrupt flag
    CTIMER_ClearStatusFlags(CTIMER2, kCTIMER_Match0Flag);

    // Trigger ADC conversion
    LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
}
