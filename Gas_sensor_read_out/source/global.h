// global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_ARRAY_SIZE 12500
#define SAMPLE_RATE 25
#define OPAMP_GAIN 7
#define DEMO_DAC14_BASEADDR DAC2
#define LOWEST_FREQ_POSSIBLE 0.002 // in Hz so period of 500s

// Define the maximum number of strips
#define MAX_STRIP_COUNT 8

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile uint32_t g_Dac14InputIndex;
extern uint32_t g_Dac14Values[MAX_ARRAY_SIZE];
extern uint32_t g_Dac14ValueArraySize;
extern uint32_t waveType;
extern uint32_t dutyCycle;
extern float desiredFrequency;
extern uint32_t desiredAmplitude;

// flag to indicate g_strip array is filled
extern volatile bool g_strip_values_ready;

// Declare the external ADC value variable
extern volatile uint16_t g_AdcValue;

// Variable to keep track of the number of graphene strips to be read
extern uint8_t g_strip_count;

// 2D Array to store the read values from the ADC, corresponding strip number, and in the future the timestamp
extern uint32_t g_strip_values[MAX_STRIP_COUNT][2];

// Variable to keep track of the timestamp in ms
extern volatile uint32_t g_timestamp_ms;


#endif // GLOBAL_H
