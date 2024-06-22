#ifndef MODBUS_H
#define MODBUS_H

// #include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "source/global.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MODBUS_LPUART          LPUART2
#define DEMO_LPUART          LPUART4
#define DEMO_LPUART_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(4u)
#define MODBUS_LPUART_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(2u)
#define MODBUS_LPUART_CLK_ATTACH kFRO12M_to_FLEXCOMM2
#define DEMO_LPUART_CLK_ATTACH kFRO12M_to_FLEXCOMM4

#define FUNC_READ_HOLDING_REGISTERS 0x03
#define FUNC_READ_INPUT_REGISTERS 0x04
#define FUNC_WRITE_SINGLE_REGISTER 0x06
#define FUNC_WRITE_MULTIPLE_REGISTERS 0x10
#define ERROR_ILLEGAL_FUNCTION 0x01
#define ERROR_ILLEGAL_DATA_ADDRESS 0x02
#define ERROR_ILLEGAL_DATA_VALUE 0x03

#define MODBUS_ADDRESS        0x01
#define INPUT_REGISTER_COUNT 8
#define HOLDING_REGISTER_COUNT 7
#define MODBUS_BUFFER_LENGTH 256

/*******************************************************************************************
 * Parameter (holding) registers indices --> 0: wavetype, 1: Duty cycle, 2: desiredFrequency
 * 3: desiredAmplitude, 4: excitationVoltagePerStrip, 5: mux freq, 6: active strips(0-8)
 * 
 * active strips bit # --> strip 1: bit 0, strip 2: bit 1, ..., strip 8: bit 7
 *******************************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Modbus_init_UARTs(void);
void HandleModbusFrame(LPUART_Type *modbus_uart);
void Modbus_HandleRequest(uint8_t *buffer, uint16_t length);
void Modbus_HandleReadHoldingRegister(uint8_t *buffer, uint16_t length);
void Modbus_HandleReadInputRegister(uint8_t *buffer, uint16_t length);
void Modbus_HandleWriteSingleRegister(uint8_t *buffer, uint16_t length);
void Modbus_HandleWriteMultipleRegisters(uint8_t *buffer, uint16_t length);
void Modbus_SendError(uint8_t address, uint8_t function, uint8_t errorCode);
uint16_t Modbus_CRC16(const uint8_t *nData, uint16_t wLength);

void Modbus_UpdateParameters(uint16_t *buffer);
bool Modbus_DataErrorPresent(uint16_t *buffer);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint16_t inputRegisters[INPUT_REGISTER_COUNT];
extern uint16_t holdingRegisters[HOLDING_REGISTER_COUNT];

#endif // MODBUS_H
