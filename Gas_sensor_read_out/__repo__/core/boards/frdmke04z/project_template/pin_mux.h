/*
 * Copyright 2018 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTB3 (number 13), J1[10]/J2[10]/J4[3]/D4[3]/PTB3_D12_SPI0_MOSI/PTB3_BLUE
  @{ */
#define BOARD_INITLEDSPINS_LED_BLUE_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITLEDSPINS_LED_BLUE_GPIO_PORT kGPIO_PORTB /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITLEDSPINS_LED_BLUE_PORT PORTB            /*!<@brief PORT device name: PORTB */
#define BOARD_INITLEDSPINS_LED_BLUE_PIN 3U                /*!<@brief PORTB pin index: 3 */
                                                          /* @} */

/*! @name PORTC4 (number 2), J1[8]/D4[4]/PTC4_GREEN
  @{ */
#define BOARD_INITLEDSPINS_LED_GREEN_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITLEDSPINS_LED_GREEN_GPIO_PORT kGPIO_PORTC /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITLEDSPINS_LED_GREEN_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_INITLEDSPINS_LED_GREEN_PIN 4U                /*!<@brief PORTC pin index: 4 */
                                                           /* @} */

/*! @name PORTC5 (number 1), J1[12]/D4[1]/PTC5_RED
  @{ */
#define BOARD_INITLEDSPINS_LED_RED_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITLEDSPINS_LED_RED_GPIO_PORT kGPIO_PORTC /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITLEDSPINS_LED_RED_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_INITLEDSPINS_LED_RED_PIN 5U                /*!<@brief PORTC pin index: 5 */
                                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

/*! @name PORTB5 (number 7), J2[6]/PTB5_TSI
  @{ */
#define BOARD_INITTOUCHPINS_PTB5_TSI_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITTOUCHPINS_PTB5_TSI_GPIO_PORT kGPIO_PORTB /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITTOUCHPINS_PTB5_TSI_PORT PORTB            /*!<@brief PORT device name: PORTB */
#define BOARD_INITTOUCHPINS_PTB5_TSI_PIN 5U                /*!<@brief PORTB pin index: 5 */
                                                           /* @} */

/*! @name PORTC1 (number 11), J2[3]/PTC1_TSI/PTC1_PWM1
  @{ */
#define BOARD_INITTOUCHPINS_PTC1_TSI_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITTOUCHPINS_PTC1_TSI_GPIO_PORT kGPIO_PORTC /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITTOUCHPINS_PTC1_TSI_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_INITTOUCHPINS_PTC1_TSI_PIN 1U                /*!<@brief PORTC pin index: 1 */
                                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTOUCHPins(void);

/*! @name PORTB0 (number 16), J1[2]/UART0_RX_TGTMCU
  @{ */
#define BOARD_INITDEBUG_UARTPINS_DEBUG_UART_RX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITDEBUG_UARTPINS_DEBUG_UART_RX_PIN 0U     /*!<@brief PORTB pin index: 0 */
                                                          /* @} */

/*! @name PORTB1 (number 15), J1[4]/PTB1_IRTX/UART0_TX_TGTMCU
  @{ */
#define BOARD_INITDEBUG_UARTPINS_DEBUG_UART_TX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITDEBUG_UARTPINS_DEBUG_UART_TX_PIN 1U     /*!<@brief PORTB pin index: 1 */
                                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name PORTA3 (number 19), J2[20]/J4[7]/PTA3_D15_ANB0/PTA3_ACCEL_SCL
  @{ */
#define BOARD_INITACCELPINS_ACCEL_SCL_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITACCELPINS_ACCEL_SCL_PIN 3U     /*!<@brief PORTA pin index: 3 */
                                                 /* @} */

/*! @name PORTA2 (number 20), J1[11]/J2[18]/PTA2_D14_T2/PTA2_ACCEL_SDA
  @{ */
#define BOARD_INITACCELPINS_ACCEL_SDA_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITACCELPINS_ACCEL_SDA_PIN 2U     /*!<@brief PORTA pin index: 2 */
                                                 /* @} */

/*! @name PORTC0 (number 12), J2[4]/PTC0_ACCEL_INT1
  @{ */
#define BOARD_INITACCELPINS_ACCEL_INT1_GPIO GPIOA            /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITACCELPINS_ACCEL_INT1_GPIO_PORT kGPIO_PORTC /*!<@brief PORTA GPIO port: PORTA */
#define BOARD_INITACCELPINS_ACCEL_INT1_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_INITACCELPINS_ACCEL_INT1_PIN 0U                /*!<@brief PORTC pin index: 0 */
                                                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

/*! @name PORTB7 (number 5), J1[9]/PTB7_GPIO/EXTAL
  @{ */
#define BOARD_INITOSCPINS_EXTAL_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITOSCPINS_EXTAL_PIN 7U     /*!<@brief PORTB pin index: 7 */
                                           /* @} */

/*! @name PORTB6 (number 6), J1[15]/PTB6_LED3/XTAL
  @{ */
#define BOARD_INITOSCPINS_XTAL_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITOSCPINS_XTAL_PIN 6U     /*!<@brief PORTB pin index: 6 */
                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
