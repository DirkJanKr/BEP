/*
 * Copyright 2017-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v8.0
processor: LPC55S06
package_id: LPC55S06JBD64
mcu_data: ksdk2_0
processor_version: 0.9.5
board: LPCXpresso55S06
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitDEBUG_UARTPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '61', peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI_DATA/CTIMER2_MAT3/SCT0_OUT8/CMP0_OUT/PLU_OUT2/SECURE_GPIO0_29,
    mode: inactive, slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '63', peripheral: FLEXCOMM0, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_30/FC0_TXD_SCL_MISO_WS/CTIMER0_MAT0/SCT0_OUT9/SECURE_GPIO0_30, mode: inactive,
    slew_rate: standard, invert: disabled, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitDEBUG_UARTPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port0_pin29_config = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
                                         IOCON_PIO_FUNC1 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN29 (coords: 61) is configured as FC0_RXD_SDA_MOSI_DATA */
    IOCON_PinMuxSet(IOCON, 0U, 29U, port0_pin29_config);

    const uint32_t port0_pin30_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
                                         IOCON_PIO_FUNC1 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN30 (coords: 63) is configured as FC0_TXD_SCL_MISO_WS */
    IOCON_PinMuxSet(IOCON, 0U, 30U, port0_pin30_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSWD_DEBUGPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '13', peripheral: SWD, signal: SWO, pin_signal: PIO0_10/FC6_SCK/CT_INP10/CTIMER2_MAT0/FC1_TXD_SCL_MISO_WS/SCT0_OUT2/SWO/SECURE_GPIO0_10/ADC0_1, mode: inactive,
    slew_rate: standard, invert: disabled, open_drain: disabled, asw: disabled}
  - {pin_num: '6', peripheral: SWD, signal: SWCLK, pin_signal: PIO0_11/FC6_RXD_SDA_MOSI_DATA/CTIMER2_MAT2/FREQME_GPIO_CLK_A/SWCLK/SECURE_GPIO0_11/ADC0_9, mode: pullDown,
    slew_rate: standard, invert: disabled, open_drain: disabled, asw: disabled}
  - {pin_num: '5', peripheral: SWD, signal: SWDIO, pin_signal: PIO0_12/FC3_TXD_SCL_MISO_WS/FREQME_GPIO_CLK_B/SCT_GPI7/SWDIO/FC6_TXD_SCL_MISO_WS/SECURE_GPIO0_12/ADC0_10,
    mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled, asw: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSWD_DEBUGPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitSWD_DEBUGPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port0_pin10_config = (/* Pin is configured as SWO */
                                         IOCON_PIO_FUNC6 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI |
                                         /* Analog switch is open (disabled) */
                                         IOCON_PIO_ASW_DI);
    /* PORT0 PIN10 (coords: 13) is configured as SWO */
    IOCON_PinMuxSet(IOCON, 0U, 10U, port0_pin10_config);

    const uint32_t port0_pin11_config = (/* Pin is configured as SWCLK */
                                         IOCON_PIO_FUNC6 |
                                         /* Selects pull-down function */
                                         IOCON_PIO_MODE_PULLDOWN |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI |
                                         /* Analog switch is open (disabled) */
                                         IOCON_PIO_ASW_DI);
    /* PORT0 PIN11 (coords: 6) is configured as SWCLK */
    IOCON_PinMuxSet(IOCON, 0U, 11U, port0_pin11_config);

    const uint32_t port0_pin12_config = (/* Pin is configured as SWDIO */
                                         IOCON_PIO_FUNC6 |
                                         /* Selects pull-up function */
                                         IOCON_PIO_MODE_PULLUP |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI |
                                         /* Analog switch is open (disabled) */
                                         IOCON_PIO_ASW_DI);
    /* PORT0 PIN12 (coords: 5) is configured as SWDIO */
    IOCON_PinMuxSet(IOCON, 0U, 12U, port0_pin12_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: GPIO, signal: 'PIO0, 18', pin_signal: PIO0_18/FC4_CTS_SDA_SSEL0/CTIMER1_MAT0/SCT0_OUT1/PLU_IN3/SECURE_GPIO0_18/ACMP0_C, direction: OUTPUT,
    gpio_init_state: 'false', mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled, asw: disabled}
  - {pin_num: '49', peripheral: GPIO, signal: 'PIO0, 21', pin_signal: PIO0_21/FC3_RTS_SCL_SSEL1/UTICK_CAP3/CTIMER3_MAT3/SCT_GPI3/FC7_SCK/HS_SPI_SSEL3/PLU_CLKIN/SECURE_GPIO0_21,
    direction: OUTPUT, gpio_init_state: 'false', mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '51', peripheral: GPIO, signal: 'PIO0, 22', pin_signal: PIO0_22/FC6_TXD_SCL_MISO_WS/UTICK_CAP1/CT_INP15/SCT0_OUT3/PLU_OUT7/SECURE_GPIO0_22, direction: OUTPUT,
    gpio_init_state: 'false', mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitLEDsPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_18 (pin 38)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_GREEN_GPIO, BOARD_INITLEDSPINS_LED_GREEN_PORT, BOARD_INITLEDSPINS_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_21 (pin 49)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_22 (pin 51)  */
    GPIO_PinInit(BOARD_INITLEDSPINS_LED_BLUE_GPIO, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, &LED_BLUE_config);

    const uint32_t LED_GREEN = (/* Pin is configured as PIO0_18 */
                                IOCON_PIO_FUNC0 |
                                /* Selects pull-up function */
                                IOCON_PIO_MODE_PULLUP |
                                /* Standard mode, output slew rate control is enabled */
                                IOCON_PIO_SLEW_STANDARD |
                                /* Input function is not inverted */
                                IOCON_PIO_INV_DI |
                                /* Enables digital function */
                                IOCON_PIO_DIGITAL_EN |
                                /* Open drain is disabled */
                                IOCON_PIO_OPENDRAIN_DI |
                                /* Analog switch is open (disabled) */
                                IOCON_PIO_ASW_DI);
    /* PORT0 PIN18 (coords: 38) is configured as PIO0_18 */
    IOCON_PinMuxSet(IOCON, BOARD_INITLEDSPINS_LED_GREEN_PORT, BOARD_INITLEDSPINS_LED_GREEN_PIN, LED_GREEN);

    const uint32_t LED_RED = (/* Pin is configured as PIO0_21 */
                              IOCON_PIO_FUNC0 |
                              /* Selects pull-up function */
                              IOCON_PIO_MODE_PULLUP |
                              /* Standard mode, output slew rate control is enabled */
                              IOCON_PIO_SLEW_STANDARD |
                              /* Input function is not inverted */
                              IOCON_PIO_INV_DI |
                              /* Enables digital function */
                              IOCON_PIO_DIGITAL_EN |
                              /* Open drain is disabled */
                              IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN21 (coords: 49) is configured as PIO0_21 */
    IOCON_PinMuxSet(IOCON, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, LED_RED);

    const uint32_t LED_BLUE = (/* Pin is configured as PIO0_22 */
                               IOCON_PIO_FUNC0 |
                               /* Selects pull-up function */
                               IOCON_PIO_MODE_PULLUP |
                               /* Standard mode, output slew rate control is enabled */
                               IOCON_PIO_SLEW_STANDARD |
                               /* Input function is not inverted */
                               IOCON_PIO_INV_DI |
                               /* Enables digital function */
                               IOCON_PIO_DIGITAL_EN |
                               /* Open drain is disabled */
                               IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN22 (coords: 51) is configured as PIO0_22 */
    IOCON_PinMuxSet(IOCON, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, LED_BLUE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONsPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '3', peripheral: GPIO, signal: 'PIO1, 9', pin_signal: PIO1_9/FC1_SCK/CT_INP4/SCT0_OUT2/FC4_CTS_SDA_SSEL0/ADC0_12, identifier: USR, direction: INPUT,
    mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled, asw: disabled}
  - {pin_num: '44', peripheral: GPIO, signal: 'PIO0, 28', pin_signal: PIO0_28/FC0_SCK/CT_INP11/SCT0_OUT7/PLU_OUT1/SECURE_GPIO0_28, identifier: WAKEUP, direction: INPUT,
    mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '58', peripheral: GPIO, signal: 'PIO0, 5', pin_signal: PIO0_5/CAN0_TD/FC4_RXD_SDA_MOSI_DATA/CTIMER3_MAT0/SCT_GPI5/FC3_RTS_SCL_SSEL1/MCLK/SECURE_GPIO0_5,
    identifier: ISP, direction: INPUT, mode: pullUp, slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '23', peripheral: SYSCON, signal: RESET, pin_signal: RESETN, identifier: RESET}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitBUTTONsPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* Enables the clock for the GPIO1 module */
    CLOCK_EnableClock(kCLOCK_Gpio1);

    gpio_pin_config_t ISP_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_5 (pin 58)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_ISP_GPIO, BOARD_INITBUTTONSPINS_ISP_PORT, BOARD_INITBUTTONSPINS_ISP_PIN, &ISP_config);

    gpio_pin_config_t WAKEUP_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_28 (pin 44)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_WAKEUP_GPIO, BOARD_INITBUTTONSPINS_WAKEUP_PORT, BOARD_INITBUTTONSPINS_WAKEUP_PIN, &WAKEUP_config);

    gpio_pin_config_t USR_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_9 (pin 3)  */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_USR_GPIO, BOARD_INITBUTTONSPINS_USR_PORT, BOARD_INITBUTTONSPINS_USR_PIN, &USR_config);

    const uint32_t WAKEUP = (/* Pin is configured as PIO0_28 */
                             IOCON_PIO_FUNC0 |
                             /* Selects pull-up function */
                             IOCON_PIO_MODE_PULLUP |
                             /* Standard mode, output slew rate control is enabled */
                             IOCON_PIO_SLEW_STANDARD |
                             /* Input function is not inverted */
                             IOCON_PIO_INV_DI |
                             /* Enables digital function */
                             IOCON_PIO_DIGITAL_EN |
                             /* Open drain is disabled */
                             IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN28 (coords: 44) is configured as PIO0_28 */
    IOCON_PinMuxSet(IOCON, BOARD_INITBUTTONSPINS_WAKEUP_PORT, BOARD_INITBUTTONSPINS_WAKEUP_PIN, WAKEUP);

    const uint32_t ISP = (/* Pin is configured as PIO0_5 */
                          IOCON_PIO_FUNC0 |
                          /* Selects pull-up function */
                          IOCON_PIO_MODE_PULLUP |
                          /* Standard mode, output slew rate control is enabled */
                          IOCON_PIO_SLEW_STANDARD |
                          /* Input function is not inverted */
                          IOCON_PIO_INV_DI |
                          /* Enables digital function */
                          IOCON_PIO_DIGITAL_EN |
                          /* Open drain is disabled */
                          IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN5 (coords: 58) is configured as PIO0_5 */
    IOCON_PinMuxSet(IOCON, BOARD_INITBUTTONSPINS_ISP_PORT, BOARD_INITBUTTONSPINS_ISP_PIN, ISP);

    const uint32_t USR = (/* Pin is configured as PIO1_9 */
                          IOCON_PIO_FUNC0 |
                          /* Selects pull-up function */
                          IOCON_PIO_MODE_PULLUP |
                          /* Standard mode, output slew rate control is enabled */
                          IOCON_PIO_SLEW_STANDARD |
                          /* Input function is not inverted */
                          IOCON_PIO_INV_DI |
                          /* Enables digital function */
                          IOCON_PIO_DIGITAL_EN |
                          /* Open drain is disabled */
                          IOCON_PIO_OPENDRAIN_DI |
                          /* Analog switch is open (disabled) */
                          IOCON_PIO_ASW_DI);
    /* PORT1 PIN9 (coords: 3) is configured as PIO1_9 */
    IOCON_PinMuxSet(IOCON, BOARD_INITBUTTONSPINS_USR_PORT, BOARD_INITBUTTONSPINS_USR_PIN, USR);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitACCELPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '45', peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_24/FC0_RXD_SDA_MOSI_DATA/CT_INP8/SCT_GPI0/SECURE_GPIO0_24, mode: pullUp, slew_rate: standard,
    invert: disabled, open_drain: disabled}
  - {pin_num: '52', peripheral: FLEXCOMM0, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_25/FC0_TXD_SCL_MISO_WS/CT_INP9/SCT_GPI1/SECURE_GPIO0_25, mode: pullUp, slew_rate: standard,
    invert: disabled, open_drain: disabled}
  - {pin_num: '27', peripheral: GPIO, signal: 'PIO1, 23', pin_signal: PIO1_23/FC2_SCK/SCT0_OUT0/FC3_SSEL2/PLU_OUT5, mode: inactive, slew_rate: standard, invert: disabled,
    open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitACCELPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitACCELPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port0_pin24_config = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
                                         IOCON_PIO_FUNC1 |
                                         /* Selects pull-up function */
                                         IOCON_PIO_MODE_PULLUP |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN24 (coords: 45) is configured as FC0_RXD_SDA_MOSI_DATA */
    IOCON_PinMuxSet(IOCON, 0U, 24U, port0_pin24_config);

    const uint32_t port0_pin25_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
                                         IOCON_PIO_FUNC1 |
                                         /* Selects pull-up function */
                                         IOCON_PIO_MODE_PULLUP |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN25 (coords: 52) is configured as FC0_TXD_SCL_MISO_WS */
    IOCON_PinMuxSet(IOCON, 0U, 25U, port0_pin25_config);

    const uint32_t ACCL_INTR = (/* Pin is configured as PIO1_23 */
                                IOCON_PIO_FUNC0 |
                                /* No addition pin function */
                                IOCON_PIO_MODE_INACT |
                                /* Standard mode, output slew rate control is enabled */
                                IOCON_PIO_SLEW_STANDARD |
                                /* Input function is not inverted */
                                IOCON_PIO_INV_DI |
                                /* Enables digital function */
                                IOCON_PIO_DIGITAL_EN |
                                /* Open drain is disabled */
                                IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN23 (coords: 27) is configured as PIO1_23 */
    IOCON_PinMuxSet(IOCON, BOARD_INITACCELPINS_ACCL_INTR_PORT, BOARD_INITACCELPINS_ACCL_INTR_PIN, ACCL_INTR);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCANPins:
- options: {callFromInitBoot: 'false', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '41', peripheral: CAN0, signal: CAN_TD, pin_signal: PIO1_2/CAN0_TD/CTIMER0_MAT3/SCT_GPI6/HS_SPI_SCK/PLU_OUT5, mode: inactive, slew_rate: standard, invert: disabled,
    open_drain: disabled}
  - {pin_num: '42', peripheral: CAN0, signal: CAN_RD, pin_signal: PIO1_3/CAN0_RD/SCT0_OUT4/HS_SPI_MISO/PLU_OUT6, mode: inactive, slew_rate: standard, invert: disabled,
    open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitCANPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitCANPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port1_pin2_config = (/* Pin is configured as CAN0_TD */
                                        IOCON_PIO_FUNC1 |
                                        /* No addition pin function */
                                        IOCON_PIO_MODE_INACT |
                                        /* Standard mode, output slew rate control is enabled */
                                        IOCON_PIO_SLEW_STANDARD |
                                        /* Input function is not inverted */
                                        IOCON_PIO_INV_DI |
                                        /* Enables digital function */
                                        IOCON_PIO_DIGITAL_EN |
                                        /* Open drain is disabled */
                                        IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN2 (coords: 41) is configured as CAN0_TD */
    IOCON_PinMuxSet(IOCON, 1U, 2U, port1_pin2_config);

    const uint32_t port1_pin3_config = (/* Pin is configured as CAN0_RD */
                                        IOCON_PIO_FUNC1 |
                                        /* No addition pin function */
                                        IOCON_PIO_MODE_INACT |
                                        /* Standard mode, output slew rate control is enabled */
                                        IOCON_PIO_SLEW_STANDARD |
                                        /* Input function is not inverted */
                                        IOCON_PIO_INV_DI |
                                        /* Enables digital function */
                                        IOCON_PIO_DIGITAL_EN |
                                        /* Open drain is disabled */
                                        IOCON_PIO_OPENDRAIN_DI);
    /* PORT1 PIN3 (coords: 42) is configured as CAN0_RD */
    IOCON_PinMuxSet(IOCON, 1U, 3U, port1_pin3_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
