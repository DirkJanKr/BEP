#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_ARRAY_SIZE 10000
#define SAMPLE_RATE 1000
#define OPAMP_GAIN 1
#define DEMO_DAC14_BASEADDR DAC2


/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile uint32_t g_Dac14InputIndex;
extern uint32_t g_Dac14Values[MAX_ARRAY_SIZE];
extern uint32_t g_Dac14ValueArraySize;
extern uint32_t waveType;

extern uint32_t dutyCycle;
extern uint32_t desiredFrequency;
extern uint32_t desiredAmplitude;

#endif // GLOBAL_H
