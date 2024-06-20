#ifndef ADC_16_H
#define ADC_16_H

#include <stdint.h>

// Function to initialize the ADC
void ADC_Voltage_Initialize(void);

void ADC_Voltage_timer_setup(void);

void ADC_Current_Initialize(void);

int FirstActiveStrip(void);

void ConvertActiveStrips(void);
#endif
