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
extern volatile bool I_sens_strip_values_ready;
extern volatile bool V_sens_strip_values_ready;


// Declare the external ADC value variable
extern volatile uint16_t V_sens_AdcValue;
extern volatile uint16_t I_sens_AdcValue;


// Variable to keep track of the number of graphene strips to be read
extern uint8_t g_strip_count;


// 2D Array to store the read values from the ADC, corresponding strip number, and in the future the timestamp
extern uint32_t V_sens_strip_values[MAX_STRIP_COUNT][2];
extern uint32_t I_sens_strip_values[MAX_STRIP_COUNT][2];

extern int resistance_array[8][2];


// Variable to keep track of the timestamp in ms
extern volatile uint32_t g_timestamp_ms;

/*MUX parameters*/
// Global parameters
extern uint8_t MUX_enable;
extern uint8_t MUX_channel;
extern uint16_t MUX_freq;

/*DAC parameters for the excitation voltage*/
extern bool g_excitation_voltage_on;
extern float g_excitation_voltage_per_resistor;
extern uint8_t g_resistor_formation;

extern bool active_strips[8];

// Value to store the result of the curr strip
extern uint32_t current_adc_result;

// Function prototypes

#endif // GLOBAL_H
