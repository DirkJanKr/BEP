#ifndef ADC_16_H
#define ADC_16_H

#include <stdint.h>

// Function to initialize the ADC
void ADC_Initialize(void);

void ADC_timer_setup(float mux_freq);

void ADC_Callback(uint32_t flags);

void ADC_ContinuousRead(void);

#endif
