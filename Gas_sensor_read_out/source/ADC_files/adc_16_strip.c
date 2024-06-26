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
#include "source/MUX_files/gpio_for_mux.h"
#include <stdlib.h> // Include for memory allocation

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define Voltage_SPC_BASE               SPC0
#define Voltage_LPADC_BASE             ADC0
#define Voltage_LPADC_IRQn             ADC0_IRQn
#define Voltage_LPADC_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define Voltage_LPADC_USER_CHANNEL     2U
#define Voltage_LPADC_USER_CMDID       1U
#define Voltage_LPADC_VREF_SOURCE      kLPADC_ReferenceVoltageAlt3
#define Voltage_LPADC_DO_OFFSET_CALIBRATION true
#define Voltage_LPADC_OFFSET_VALUE_A   0x10U
#define Voltage_LPADC_OFFSET_VALUE_B   0x10U
#define Voltage_LPADC_USE_HIGH_RESOLUTION   true
#define Current_LPADC_BASE             ADC1

#define Voltage_VREF_BASE VREF0

#define Current_SPC_BASE               SPC0
#define Current_LPADC_BASE             ADC1
#define Current_LPADC_IRQn             ADC1_IRQn
#define Current_LPADC_IRQ_HANDLER_FUNC ADC1_IRQHandler
#define Current_LPADC_USER_CHANNEL     6U
#define Current_LPADC_USER_CMDID       1U
#define Current_LPADC_VREF_SOURCE      kLPADC_ReferenceVoltageAlt3
#define Current_LPADC_DO_OFFSET_CALIBRATION true
#define Current_LPADC_OFFSET_VALUE_A   0x10U
#define Current_LPADC_OFFSET_VALUE_B   0x10U
#define Current_LPADC_USE_HIGH_RESOLUTION   true

#define Current_VREF_BASE VREF0

#define HardwareAveraging kLPADC_ConversionAverage1024
#define CycleSampleTime kLPADC_SampleTimeADCK131


// used when averaging ADC values
// #define AVERAGE_COUNT 100


/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool I_sens_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t I_sens_LpadcResultConfigStruct;
volatile uint16_t I_sens_AdcValue = 0;
// volatile uint32_t I_sens_HardwareAverageSum = 0;
// volatile uint16_t I_sens_HardwareAverageCount = 0;

const uint32_t I_sens_LpadcFullRange = 65536U;
const uint32_t I_sens_LpadcResultShift = 0U;

// Variable to keep track of the number of graphene strips to be read
// static uint8_t currentStrip = 0;

// Referernce voltage, I am not sure how accurate this 3.3V is and not sure how tocheck this.
float I_sens_reference_voltage = 3.3;

volatile bool V_sens_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t V_sens_LpadcResultConfigStruct;
volatile uint16_t V_sens_AdcValue = 0;

const uint32_t V_sens_LpadcFullRange = 65536U;
const uint32_t V_sens_LpadcResultShift = 0U;

// Referernce voltage, I am not sure how accurate this 3.3V is and not sure how tocheck this.
float V_sens_reference_voltage = 3.3;

// Variables to store the sum and count for current adc values
volatile uint32_t I_sens_Sum = 0;
volatile uint16_t I_sens_Count = 0;

static uint8_t SoftwareAverageCount = 30;
static uint8_t currentCountIndex = 0;
// Value to store the result of the curr strip
static uint32_t VoltageSum = 0;
static uint32_t CurrentSum = 0;
static uint32_t resultVoltage = 0;
static uint32_t current_adc_result = 0;
static uint32_t Total_averaged_current = 0;
static uint32_t Total_adc_summation = 0;

static uint8_t current_active_strip_index = 0;
static uint8_t *active_strip_indices = NULL; // Pointer to dynamically allocated array

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * ADC Interrupt Handler
 * This function is called when the ADC conversion is complete.
 * It retrieves the conversion result and prints it.
 */
void ADC0_IRQHandler(void) {
    if (LPADC_GetConvResult(Voltage_LPADC_BASE, &V_sens_LpadcResultConfigStruct, 0U)) {

        V_sens_LpadcConversionCompletedFlag = true;
        VoltageSum += (V_sens_LpadcResultConfigStruct.convValue >> V_sens_LpadcResultShift);
    }
    SDK_ISR_EXIT_BARRIER;
}

// Current ADC interrupt handler
// This function is called when the ADC conversion is complete and it will calculate the average of the ADC values
// Since the current can be asumed to be stable over time, the average of the ADC values will be used as the current value
void ADC1_IRQHandler(void) {
    if (LPADC_GetConvResult(Current_LPADC_BASE, &I_sens_LpadcResultConfigStruct, 0U)) {
        I_sens_LpadcConversionCompletedFlag = true;

        CurrentSum += (I_sens_LpadcResultConfigStruct.convValue >> I_sens_LpadcResultShift);
    }
    SDK_ISR_EXIT_BARRIER;
}

/**
 * Initialize the ADC
 * This function sets up the ADC with the required configurations and calibrations.
 */
void ADC_Voltage_Initialize(void) {
    // Enable the Vref to make a 1.2V reference voltage for the ADC
    SPC0->ACTIVE_CFG1 |= 0x1;

    vref_config_t vrefConfig_12;

    VREF_GetDefaultConfig(&vrefConfig_12);
    vrefConfig_12.bufferMode = kVREF_ModeLowPowerBuffer;
    // vrefConfig_12.enableHCBandgap = false;

    VREF_Init(VREF0, &vrefConfig_12);

    // Set voltage to 1.2V
    VREF_SetTrim21Val(VREF0, 2U);

    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;
    vref_config_t vrefConfig;

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

    SPC_EnableActiveModeAnalogModules(Voltage_SPC_BASE, kSPC_controlVref);

    VREF_GetDefaultConfig(&vrefConfig);
    vrefConfig.bufferMode = kVREF_ModeBandgapOnly;
    VREF_Init(Voltage_VREF_BASE, &vrefConfig);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    mLpadcConfigStruct.triggerPriorityPolicy = kLPADC_TriggerPriorityPreemptSubsequently;

    mLpadcConfigStruct.conversionAverageMode = HardwareAveraging;
    LPADC_Init(Voltage_LPADC_BASE, &mLpadcConfigStruct);
    LPADC_DoOffsetCalibration(Voltage_LPADC_BASE);
    LPADC_DoAutoCalibration(Voltage_LPADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.sampleTimeMode = CycleSampleTime;
    mLpadcCommandConfigStruct.channelNumber = Voltage_LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(Voltage_LPADC_BASE, Voltage_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = Voltage_LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;

    LPADC_SetConvTriggerConfig(Voltage_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct);
    LPADC_EnableInterrupts(Voltage_LPADC_BASE, kLPADC_FIFO0WatermarkInterruptEnable);
    EnableIRQWithPriority(Voltage_LPADC_IRQn, 3); // Set ADC interrupt priority to 3

    PRINTF("ADC Full Range: %d\n", V_sens_LpadcFullRange);
}

void ADC_Current_Initialize(void) {
    // Enable the Vref to make a 1.2V reference voltage for the ADC
    SPC0->ACTIVE_CFG1 |= 0x1;

    vref_config_t vrefConfig_12_cur;

    VREF_GetDefaultConfig(&vrefConfig_12_cur);
    vrefConfig_12_cur.bufferMode = kVREF_ModeLowPowerBuffer;

    VREF_Init(VREF0, &vrefConfig_12_cur);

    // Set voltage to 1.2V
    VREF_SetTrim21Val(VREF0, 2U);    

    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;
    vref_config_t vrefConfig;

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    CLOCK_SetClkDiv(kCLOCK_DivAdc1Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC1);

    SPC_EnableActiveModeAnalogModules(Current_SPC_BASE, kSPC_controlVref);

    VREF_GetDefaultConfig(&vrefConfig);
    vrefConfig.bufferMode = kVREF_ModeBandgapOnly;
    VREF_Init(Current_VREF_BASE, &vrefConfig);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    mLpadcConfigStruct.triggerPriorityPolicy = kLPADC_TriggerPriorityPreemptSubsequently;
    mLpadcConfigStruct.conversionAverageMode = HardwareAveraging;
    LPADC_Init(Current_LPADC_BASE, &mLpadcConfigStruct);

    LPADC_DoOffsetCalibration(Current_LPADC_BASE);
    LPADC_DoAutoCalibration(Current_LPADC_BASE);

    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.sampleTimeMode = CycleSampleTime;
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
    mLpadcCommandConfigStruct.channelNumber = Current_LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(Current_LPADC_BASE, Current_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = Current_LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(Current_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct);
    LPADC_EnableInterrupts(Current_LPADC_BASE, kLPADC_FIFO0WatermarkInterruptEnable);

    EnableIRQWithPriority(Current_LPADC_IRQn, 3);
    PRINTF("ADC Full Range: %d\n", I_sens_LpadcFullRange);
}

/**
 * Setup the ADC timer
 * This function configures the timer to generate interrupts at the desired frequency.
 */
void ADC_Voltage_timer_setup(void) {

    // Stop the timer if it is already running
    CTIMER_StopTimer(CTIMER2);

    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2); // Attach FRO 48MHz to CTIMER2.

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER2, &config);

    uint32_t matchValue = 48000000 / (MUX_freq * SoftwareAverageCount); // 48MHz is the frequency of the FRO clock

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

    // Set current active strip and index to zero, if a new measurement is started
    current_active_strip_index = 0;
    currentCountIndex = 0;
}

void calculate_resistance_array(void) {
    // Calculate the resistance array, loop through the active strips array and calculate the resistance of active strips
    // Leave the false entries at 0. Also add the timestamp from the V_sens_strip_values array
    for (int i = 0; i < MAX_STRIP_COUNT; i++) {
        if (active_strips[i]) {
            // Fill in the timestamp in the resistance array
            resistance_array[i][1] = V_sens_strip_values[i][1];
            Total_averaged_current = Total_adc_summation / g_strip_count;
            // PRINTF("Total adc summation: %d\n", Total_adc_summation);
            // PRINTF("Total averaged current: %d\n", Total_averaged_current);

            // Calculate the resistance and round to the nearest integer
            // 0.5 is added to account for truncation when casting to int

            resistance_array[i][0] = (int)(((float)V_sens_strip_values[i][0] / ((float)Total_averaged_current) * 120 * 75 + 0.5));

   
        }
    }
    Total_adc_summation = 0;
    Total_averaged_current = 0;

}

void ConvertActiveStrips() {
    // Free any previously allocated memory
    if (active_strip_indices != NULL) {
        free(active_strip_indices);
    }

    // Allocate memory for active_strip_indices based on g_strip_count
    active_strip_indices = (uint8_t *)malloc(g_strip_count * sizeof(uint8_t));
    if (active_strip_indices == NULL) {
        PRINTF("Failed to allocate memory for active_strip_indices\n");
        return;
    }
    // print the active strips array for debugging
    // for (uint8_t i = 0; i < 8; i++) {
    //     PRINTF("Active strips: %d\n", active_strips[i]);
    // }
    uint8_t index = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (active_strips[i]) {
            active_strip_indices[index++] = i;
        }
    }
}


int FirstActiveStrip() {
    for (uint8_t i = 0; i < 8; i++) {
        if (active_strips[i]) {
            PRINTF("First active strip: %d\n", i);
            return i;
        }
    }
    return -1;
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



    if (g_strip_count == 0 && !ModbusFlag) {
        PRINTF("No active strips\n");
        return;
    }

    if (currentCountIndex < (SoftwareAverageCount - 1)) {
        LPADC_DoSoftwareTrigger(Voltage_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        for (volatile int i = 0; i < 150; i++); // Small delay
        LPADC_DoSoftwareTrigger(Current_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        currentCountIndex++;

    } else {
        LPADC_DoSoftwareTrigger(Voltage_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        for (volatile int i = 0; i < 150; i++); // Small delay
        LPADC_DoSoftwareTrigger(Current_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        // calculate the average of the ADC values
        current_adc_result = CurrentSum / SoftwareAverageCount;
        resultVoltage = VoltageSum / SoftwareAverageCount;
        Total_adc_summation += current_adc_result;



        // PRINTF("Active channel: %d\n", active_strip_indices[current_active_strip_index]);
        uint8_t currentChannel = active_strip_indices[current_active_strip_index];
        // PRINTF("currentChannel: %d\n", currentChannel);
        // PRINTF("g_tiemstamp_ms: %d\n", g_timestamp_ms);
        // print the current channel and the timestamp
        // PRINTF("Current channel: %d, Time: %d\n", currentChannel, g_timestamp_ms);

        V_sens_strip_values[currentChannel][0] = resultVoltage;
        V_sens_strip_values[currentChannel][1] = g_timestamp_ms; 

        CurrentSum = 0;
        VoltageSum = 0;
        currentCountIndex = 0;

        current_active_strip_index = (current_active_strip_index + 1) % g_strip_count;

        // SelectMuxChannel(active_strip_indices[current_active_strip_index]);

        if (current_active_strip_index == 0) {
            // current_adc_result = current_adc_result / g_strip_count;
            // Calculate the resistance array
            calculate_resistance_array();
            // PRINTF("current_adc_result: %d\n", current_adc_result);
            // Print the resistance array
            for (int i = 0; i < 8; i++) {
                PRINTF("Resistance of strip %d: %d, Time: %d\n", i + 1, resistance_array[i][0], resistance_array[i][1]);
            };
        }
        SelectMuxChannel(active_strip_indices[current_active_strip_index]);

    }
}
