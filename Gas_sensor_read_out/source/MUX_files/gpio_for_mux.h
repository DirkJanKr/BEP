// global.h

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>
#include "source/global.h"

// Function prototypes
void InitMuxGpio(void);
void ProcessMuxControl(void);
void SelectMuxChannel(uint8_t MUX_channel);
void EnableMux(void);
void DisableMux(void);

#define BOARD_INITPINS_GPIO_PORT GPIO4
#define GPIO_PIN_S0 12U  // Pin P4_12
#define GPIO_PIN_S1 14U  // Pin P4_14
#define GPIO_PIN_S2 16U  // Pin P4_16
#define GPIO_PIN_ENABLE 18U  // Pin P4_18

#endif // _GLOBAL_H_
