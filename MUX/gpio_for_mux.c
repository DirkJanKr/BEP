#include "fsl_gpio.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fsl_port.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_INITPINS_GPIO_PORT GPIO4
#define GPIO_PIN_S0 12U  // Pin P4_12
#define GPIO_PIN_S1 14U  // Pin P4_14
#define GPIO_PIN_S2 16U  // Pin P4_16
#define GPIO_PIN_ENABLE 18U  // Pin P4_18

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void InitMuxGpio(void);
void SelectMuxChannel(uint8_t channel);
void EnableMux(void);
void DisableMux(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

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
 * @param channel The desired channel to select (1-8).
 */
void SelectMuxChannel(uint8_t channel)
{
    // Adjust the channel range to 0-7 internally
    uint8_t adjustedChannel = channel - 1;

    // Set GPIO pins based on the channel number
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S0, (adjustedChannel & 0x01));
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S1, (adjustedChannel & 0x02) >> 1);
    GPIO_PinWrite(BOARD_INITPINS_GPIO_PORT, GPIO_PIN_S2, (adjustedChannel & 0x04) >> 2);

    PRINTF("Selected MUX channel: %d\r\n", channel);

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


/*!
 * @brief Main function
 */
int main(void)
{
    // Initialize the hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    PRINTF("MUX36S08 GPIO Control Example\n");

    // Initialize MUX GPIO pins
    InitMuxGpio();

    while (1)
    {
        uint8_t enableChar;
        uint8_t channelChar;

        PRINTF("Enter command (1X for enable and 0X for disable, where X is the channel number 1-8): ");
        
        enableChar = GETCHAR();
        channelChar = GETCHAR();
        
        uint8_t enable = enableChar - '0';
        uint8_t channel = channelChar - '0';

        PRINTF("%c%c\n", enableChar, channelChar);  // Echo the input

        if ((enable == 1 || enable == 0) && (channel >= 1 && channel <= 8))
        {
            if (enable == 1)
            {
                SelectMuxChannel(channel);
                EnableMux();
            }
            else if (enable == 0)
            {
                DisableMux();
            }
        }
        else
        {
            PRINTF("Invalid input. Please enter 1X for enable and 0X for disable, where X is the channel number 1-8.\r\n");
        }
    }
}