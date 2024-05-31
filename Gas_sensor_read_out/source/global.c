// global.c
#include "global.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex = 0U;
uint32_t g_Dac14Values[MAX_ARRAY_SIZE];
uint32_t g_Dac14ValueArraySize;
uint32_t waveType = 0;  // 0: sine, 1: square, 2: triangle (or sawtooth depending on the duty cycle)
uint32_t dutyCycle = 50;    // in percentage
float desiredFrequency = 0.005;  // in Hz
uint32_t desiredAmplitude = 21;  // in volts

// flag to indicate g_strip array is filled
volatile bool g_strip_values_ready = false;

// Declare the ADC value variable
volatile uint16_t g_AdcValue = 0;

// Variable to keep track of the number of graphene strips to be read
uint8_t g_strip_count = MAX_STRIP_COUNT;

// 2D Array to store the read values from the ADC, corresponding strip number, and in the future the timestamp
uint32_t g_strip_values[MAX_STRIP_COUNT][2];

// Variable to keep track of the timestamp in ms
volatile uint32_t g_timestamp_ms = 0;