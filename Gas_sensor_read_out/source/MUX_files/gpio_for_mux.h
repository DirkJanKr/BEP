<<<<<<< Updated upstream
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

=======
// gpio_for_mux.h

#ifndef _MUX_CONTROL_H_
#define _MUX_CONTROL_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"

// Definitions
>>>>>>> Stashed changes
#define BOARD_INITPINS_GPIO_PORT GPIO4
#define GPIO_PIN_S0 12U  // Pin P4_12
#define GPIO_PIN_S1 14U  // Pin P4_14
#define GPIO_PIN_S2 16U  // Pin P4_16
#define GPIO_PIN_ENABLE 18U  // Pin P4_18

<<<<<<< Updated upstream
#endif // _GLOBAL_H_
=======
// Global parameters
extern uint8_t MUX_enable;
extern uint8_t MUX_channel;

// Function prototypes
void InitMuxGpio(void);

void SelectMuxChannel(uint8_t channel);

void EnableMux(void);

void DisableMux(void);

#endif // _MUX_CONTROL_H_
>>>>>>> Stashed changes
