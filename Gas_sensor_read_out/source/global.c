#include "global.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex = 0U;
uint32_t g_Dac14Values[MAX_ARRAY_SIZE];
uint32_t g_Dac14ValueArraySize;
uint32_t waveType = 2;  // 0: sine, 1: square, 2: triangle (or sawtooth depending on the duty cycle)

uint32_t dutyCycle = 50;    // in percentage
uint32_t desiredFrequency = 5;  // in Hz
uint32_t desiredAmplitude = 1;  // in volts
