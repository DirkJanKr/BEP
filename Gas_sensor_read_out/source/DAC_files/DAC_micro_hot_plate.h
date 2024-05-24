#ifndef DAC_MICRO_HOT_PLATE_H
#define DAC_MICRO_HOT_PLATE_H

#include <stdint.h>
#include "source/global.h"

/*******************************************************************************/
/* Prototypes                                                                  */
/*******************************************************************************/
void generateSineWave(uint32_t *values, uint32_t size);
void generateSquareWave(uint32_t *values, uint32_t size);
void generateTriangleWave(uint32_t *values, uint32_t size);
void scaleDacValues(uint32_t *values, uint32_t size);
void dac_update_callback(uint32_t flags);
void SetupTimer();
void Dac_and_other_setup();

#endif // DAC_MICRO_HOT_PLATE_H
