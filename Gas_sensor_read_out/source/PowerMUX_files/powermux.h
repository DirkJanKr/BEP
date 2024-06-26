#ifndef _POWER_MUX_H_
#define _POWER_MUX_H_

#include "fsl_port.h"
#include "fsl_gpio.h"

/* Define the port and pin number for the Power MUX */
#define POWER_MUX_PORT PORT2
#define POWER_MUX_GPIO GPIO2
#define POWER_MUX_PIN 10U

/* Function Prototypes */
void InitPowerMux(void);
void readPowerSource(void);

#endif /* _POWER_MUX_H_ */