#include "fsl_gpio.h"
#include "source/drivers/pin_mux.h"
#include "source/drivers/clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fsl_port.h"
#include "powermux.h"
#include "source/global.h"

/*!
 * @brief Initializes the GPIO pins for the Power MUX control.
 */
/* Define the port and pin number for the Power MUX */


void InitPowerMux(void)
{
    gpio_pin_config_t gpioConfig = {
        kGPIO_DigitalInput, 0, // Change to kGPIO_DigitalInput to read the pin
    };

    /* Initialize PowerMUX pins */
    GPIO_PinInit(POWER_MUX_GPIO, POWER_MUX_PIN, &gpioConfig);

}


void readPowerSource(void) {
    /* Read the GPIO pin state to determine the selected power source */
    bool powerSource = GPIO_PinRead(POWER_MUX_GPIO, POWER_MUX_PIN);

    if (powerSource) {
        PRINTF("Power source: USB\r\n");
        ModbusFlag = false;
    } else {
        PRINTF("Power source: Modbus\r\n");
        ModbusFlag = true;
    }

}