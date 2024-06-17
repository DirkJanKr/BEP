/*****************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 ****************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************
!!GlobalInfo
product: Pins v15.0
processor: MCXN947
package_id: MCXN947VDF
mcu_data: ksdk2_0
processor_version: 15.1.0
board: FRDM-MCXN947
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS *****
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END **************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A1, peripheral: LP_FLEXCOMM4, signal: LPFLEXCOMM_P0, pin_signal: PIO1_8/WUU0_IN10/LPTMR1_ALT3/TRACE_DATA0/FC4_P0/FC5_P4/CT_INP8/SCT0_OUT2/FLEXIO0_D16/PLU_OUT0/ENET0_TXD2/I3C1_SDA/TSI0_CH17/ADC1_A8,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, pull_value: low, input_buffer: enable,
    invert_input: normal}
  - {pin_num: B1, peripheral: LP_FLEXCOMM4, signal: LPFLEXCOMM_P1, pin_signal: PIO1_9/TRACE_DATA1/FC4_P1/FC5_P5/CT_INP9/SCT0_OUT3/FLEXIO0_D17/PLU_OUT1/ENET0_TXD3/I3C1_SCL/TSI0_CH18/ADC1_A9,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: D15, peripheral: CTIMER4, signal: 'MATCH, 0', pin_signal: PIO3_2/FC7_P0/CT4_MAT0/PWM0_X0/FLEXIO0_D10/SIM1_PD, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, input_buffer: enable, invert_input: normal}
  - {pin_num: U12, peripheral: ADC0, signal: 'A, 2', pin_signal: PIO4_23/TRIG_OUT5/FC2_P6/CT2_MAT3/FLEXIO0_D31/SINC0_MCLK_OUT2/OPAMP2_OUT/ADC0_A2/ADC0_B2/ADC1_B3/CMP2_IN4P}
  - {pin_num: C3, peripheral: CTIMER2, signal: 'MATCH, 0', pin_signal: PIO1_10/TRACE_DATA2/FC4_P2/FC5_P6/CT2_MAT0/SCT0_IN2/FLEXIO0_D18/PLU_IN0/ENET0_TXER/CAN0_TXD/TSI0_CH19/ADC1_A10}
  - {pin_num: U2, peripheral: DAC2, signal: OUT, pin_signal: DAC2_OUT/ADC0_A3/ADC1_A3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS *****
 */
/* clang-format on */

/* FUNCTION ************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END **************************************/
void BOARD_InitPins(void)
{
    /* Enables the clock for PORT1: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port1);
    /* Enables the clock for PORT3: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port3);
    /* Enables the clock for PORT4: Enables clock */
    CLOCK_EnableClock(kCLOCK_Port4);

    /* PORT1_10 (pin C3) is configured as CT2_MAT0 */
    PORT_SetPinMux(PORT1, 10U, kPORT_MuxAlt4);

    PORT1->PCR[10] = ((PORT1->PCR[10] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    const port_pin_config_t DEBUG_UART_RX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Low internal pull resistor value is selected. */
                                             kPORT_LowPullResistor,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive input filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain output is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as FC4_P0 */
                                             kPORT_MuxAlt2,
                                             /* Digital input enabled */
                                             kPORT_InputBufferEnable,
                                             /* Digital input is not inverted */
                                             kPORT_InputNormal,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORT1_8 (pin A1) is configured as FC4_P0 */
    PORT_SetPinConfig(BOARD_INITPINS_DEBUG_UART_RX_PORT, BOARD_INITPINS_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);

    const port_pin_config_t DEBUG_UART_TX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Low internal pull resistor value is selected. */
                                             kPORT_LowPullResistor,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive input filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain output is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as FC4_P1 */
                                             kPORT_MuxAlt2,
                                             /* Digital input enabled */
                                             kPORT_InputBufferEnable,
                                             /* Digital input is not inverted */
                                             kPORT_InputNormal,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORT1_9 (pin B1) is configured as FC4_P1 */
    PORT_SetPinConfig(BOARD_INITPINS_DEBUG_UART_TX_PORT, BOARD_INITPINS_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);

    const port_pin_config_t port3_2_pinD15_config = {/* Internal pull-up/down resistor is disabled */
                                                     kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     kPORT_LowDriveStrength,
                                                     /* Pin is configured as CT4_MAT0 */
                                                     kPORT_MuxAlt4,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};
    /* PORT3_2 (pin D15) is configured as CT4_MAT0 */
    PORT_SetPinConfig(PORT3, 2U, &port3_2_pinD15_config);

    /* PORT4_23 (pin U12) is configured as ADC0_A2 */
    PORT_SetPinMux(PORT4, 23U, kPORT_MuxAlt0);

    PORT4->PCR[23] = ((PORT4->PCR[23] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Disables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe0));


    /* PORT4_21 (pin T11) is configured as ADC1_B6 */
    PORT_SetPinMux(PORT4, 21U, kPORT_MuxAlt0);

    PORT4->PCR[21] = ((PORT4->PCR[21] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Disables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe0));

    /*MUX pins*/

    /* PORT4_12 (pin T6) is configured as PIO4_12 */
    PORT_SetPinMux(PORT4, 12U, kPORT_MuxAlt0);

    /*LSB*/
    PORT4->PCR[12] = ((PORT4->PCR[12] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT4_14 (pin N8) is configured as PIO4_14 */
    PORT_SetPinMux(PORT4, 14U, kPORT_MuxAlt0);

    /*middle bit*/
    PORT4->PCR[14] = ((PORT4->PCR[14] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT4_16 (pin R8) is configured as PIO4_16 */
    PORT_SetPinMux(PORT4, 16U, kPORT_MuxAlt0);

    /*MSB*/
    PORT4->PCR[16] = ((PORT4->PCR[16] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /*Enable pin*/
    PORT4->PCR[18] = ((PORT4->PCR[18] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));


    /*DAC for the excitation voltge*/
    const port_pin_config_t port4_2_pinT1_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Low internal pull resistor value is selected. */
                                                    kPORT_LowPullResistor,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive input filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain output is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as DAC0_OUT */
                                                    kPORT_MuxAlt0,
                                                    /* Digital input disabled; it is required for analog functions */
                                                    kPORT_InputBufferDisable,
                                                    /* Digital input is not inverted */
                                                    kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORT4_2 (pin T1) is configured as DAC0_OUT */
    PORT_SetPinConfig(PORT4, 2U, &port4_2_pinT1_config);

    /* Modbus UART pins */

    /* PORT4_0 (pin P1) is configured as FC2_P0 --> RX */
    PORT_SetPinMux(PORT4, 0U, kPORT_MuxAlt2);
    PORT4->PCR[0] = ((PORT4->PCR[0] & (~(PORT_PCR_IBE_MASK))) | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT4_1 (pin P2) is configured as FC2_P1 --> TX */
    PORT_SetPinMux(PORT4, 1U, kPORT_MuxAlt2);
    PORT4->PCR[1] = ((PORT4->PCR[1] & (~(PORT_PCR_IBE_MASK))) | PORT_PCR_IBE(PCR_IBE_ibe1));
}
/*****************************************
 * EOF
 ****************************************/