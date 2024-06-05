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
#include "gpio_for_mux.h"
#include "source/global.h"

/*!
 * @brief Initializes the GPIO pins for the MUX36S08 control.
 */
void InitMuxGpio(void)
{
    gpio_pin_config_t gpioConfig = {
        kGPIO_DigitalOutput, 0,
    };

    /* Enable clock for PORT4 */
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Initialize GPIO pins */
    GPIO_PinInit(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S0, &gpioConfig);
    GPIO_PinInit(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S1, &gpioConfig);
    GPIO_PinInit(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S2, &gpioConfig);
    GPIO_PinInit(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_ENABLE, &gpioConfig);
}

/*!
 * @brief Sets the GPIO pins to select the desired channel on the MUX36S08.
 *
 *
 */
void SelectMuxChannel(uint8_t MUX_channel)
{
    // Adjust the channel range to 0-7 internally
    uint8_t adjustedChannel = MUX_channel - 1;

    // Set GPIO pins based on the channel number
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S0, (adjustedChannel & 0x01));
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S1, (adjustedChannel & 0x02) >> 1);
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S2, (adjustedChannel & 0x04) >> 2);

    PRINTF("Selected MUX channel: %d\r\n", MUX_channel);

    uint32_t s0State = GPIO_PinRead(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S0);
    uint32_t s1State = GPIO_PinRead(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S1);
    uint32_t s2State = GPIO_PinRead(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S2);

    PRINTF("Pin states:\r\n");
    PRINTF("S0 (P4_12): %d\r\n", s0State);
    PRINTF("S1 (P4_14): %d\r\n", s1State);
    PRINTF("S2 (P4_16): %d\r\n", s2State);
}

/*!
 * @brief Enables the MUX.
 */
void EnableMux(void)
{
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_ENABLE, 1);
    PRINTF("MUX enabled.\r\n");
}

/*!
 * @brief Disables the MUX.
 */
void DisableMux(void)
{
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_ENABLE, 0);
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S0, 0);
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S1, 0);
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S2, 0);
    PRINTF("MUX disabled. All pins are off.\r\n");
}

void ProcessMuxControl(void)
{
    if (MUX_enable)
    {
        SelectMuxChannel(MUX_channel);
        EnableMux();
    }
    else
    {
        DisableMux();
    }
}


/*To add later on in the main.c file*/

//  * @brief Main function
//  */
// int main(void)
// {
//     // Initialize the hardware
//     BOARD_InitBootPins();
//     BOARD_InitBootClocks();
//     BOARD_InitDebugConsole();
//     PRINTF("MUX36S08 GPIO Control Example\n");

//     // Initialize MUX GPIO pins
//     InitMuxGpio();

