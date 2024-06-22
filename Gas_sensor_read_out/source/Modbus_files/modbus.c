#include "modbus.h"
#include "source/global.h"
#include <math.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint16_t inputRegisters[INPUT_REGISTER_COUNT] = {0}; // Array to store input register values
uint16_t holdingRegisters[HOLDING_REGISTER_COUNT] = {1, 50, 50, 21000, 1000, 8, 8}; // Array to store holding register values
/*******************************************************************************
 * Initialize LPUART2 for MODBUS
 ******************************************************************************/
void Modbus_init_UARTs()
{
    lpuart_config_t config;

    /* attach FRO 12M to FLEXCOMM2 (modbus UART) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(MODBUS_LPUART_CLK_ATTACH);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */

    // MODBUS UART
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(MODBUS_LPUART, &config, MODBUS_LPUART_CLK_FREQ);
}


/*******************************************************************************
 * Code for handling frames
 ******************************************************************************/
void HandleModbusFrame(LPUART_Type *modbus_uart)
{
    uint8_t rxBuffer[256] = {0}; 
    uint8_t byte;
    uint16_t length = 0;
    // Continuously read bytes until we receive a complete frame
    while (LPUART_ReadBlocking(MODBUS_LPUART, &byte, 1) == kStatus_Success && !ModbusParamChanged)
    {   
        // Add byte to the buffer
        if (length < MODBUS_BUFFER_LENGTH)
        {
            rxBuffer[length++] = byte;
        }

        // Check if we have received the minimum frame size
        if (length >= 8)
        {
            // Calculate the expected frame length based on function code
            uint8_t functionCode = rxBuffer[1];
            uint16_t expectedLength = 8; // Default minimum length

            if (functionCode == FUNC_WRITE_MULTIPLE_REGISTERS)
            {
                // For Write Multiple Registers, calculate expected length
                uint16_t byteCount = rxBuffer[6];
                expectedLength = 9 + byteCount;
            }

            // Check if the received frame length matches the expected length
            if (length == expectedLength)
            {
                // Send the received frame to the debug console for inspection
                PRINTF("Received Frame: ");
                for (int i = 0; i < length; i++) {
                    PRINTF("0x%02X ", rxBuffer[i]);
                }
                PRINTF("\n");

                // Handle the complete Modbus frame
                Modbus_HandleRequest(rxBuffer, length);

                // Clear the buffer and reset length for the next frame
                memset(rxBuffer, 0, sizeof(rxBuffer));
                length = 0;
                break;
            }
        }
    }
    PRINTF("--------------CHECK2-------------\n");

}

uint16_t Modbus_CRC16(const uint8_t *nData, uint16_t wLength)
{
    static const uint16_t wCRCTable[] = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

    uint8_t nTemp;
    uint16_t wCRCWord = 0xFFFF;

    while (wLength--)
    {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord  ^= wCRCTable[nTemp & 0xFF];
    }
    return wCRCWord;
} // End: CRC16

/* Handle Modbus request for supported function codes */
void Modbus_HandleRequest(uint8_t *buffer, uint16_t length)
{   
    PRINTF("Handling frame...\n"); // Debug print
    if (length < 8) // Minimum length for a Modbus RTU frame is 8 bytes
    {
        PRINTF("Unsupported length: %d", length);
        return;
    }

    uint8_t address = buffer[0];
    uint8_t functionCode = buffer[1];

    if (address != MODBUS_ADDRESS)
    {
        PRINTF("Received Address: 0x%02X, Actual Address: 0x%02X \n", address, MODBUS_ADDRESS);
        return;
    }

    PRINTF("Received Function Code: %02X\n", functionCode); // Debug print
    
    switch (functionCode)
    {  
        case FUNC_READ_INPUT_REGISTERS:
            Modbus_HandleReadInputRegister(buffer, length);
            break;
        
        case FUNC_READ_HOLDING_REGISTERS:
            Modbus_HandleReadHoldingRegister(buffer, length);
            break;

        case FUNC_WRITE_SINGLE_REGISTER:
            Modbus_HandleWriteSingleRegister(buffer, length);
            break;
        
        case FUNC_WRITE_MULTIPLE_REGISTERS:
            Modbus_HandleWriteMultipleRegisters(buffer, length);
            break;
        
        default:
            PRINTF("Error: Illegal Funtion (error code 0x01)\n");
            Modbus_SendError(buffer[0], functionCode, ERROR_ILLEGAL_FUNCTION);
            break;
    }
}

/* Handle Modbus Read Holding Registers (Function code 0x04) */
void Modbus_HandleReadHoldingRegister(uint8_t *buffer, uint16_t length)
{
    PRINTF("Handling Read Input Registers...\n"); // Debug print

    uint16_t startAddress = (buffer[2] << 8) | buffer[3];
    uint16_t registerCount = (buffer[4] << 8) | buffer[5];
    PRINTF("Start address: 0x%02X, Number of registers: 0x%02X \n", startAddress, registerCount); // debug print

    if ((startAddress + registerCount) <= HOLDING_REGISTER_COUNT)
    {
        uint8_t responseLength = 3 + registerCount * 2 + 2; // Address + Function Code + Byte Count + Data + CRC
        uint8_t txBuffer[responseLength]; // Declare txBuffer with appropriate size

        txBuffer[0] = buffer[0]; // Address
        txBuffer[1] = buffer[1]; // Function Code
        txBuffer[2] = registerCount * 2; // Byte count

        for (uint16_t i = 0; i < registerCount; i++)
        {
            txBuffer[3 + i * 2] = (holdingRegisters[startAddress + i] >> 8) & 0xFF;  // High data byte
            txBuffer[4 + i * 2] = holdingRegisters[startAddress + i] & 0xFF;         // Low data byte
        }

        uint16_t crc = Modbus_CRC16(txBuffer, 3 + registerCount * 2);
        txBuffer[3 + registerCount * 2] = crc & 0xFF;   // low byte CRC16
        txBuffer[4 + registerCount * 2] = (crc >> 8) & 0xFF;    // high byte CRC16

        LPUART_WriteBlocking(MODBUS_LPUART, txBuffer, responseLength);

        // PRINTF("Modbus: Response: ");
        // for (int i = 0; i < sizeof(txBuffer); i++) {
        //         PRINTF("0x%02X ", txBuffer[i]);
        //     }
        // PRINTF("\n");
    }

    else
    {
        //PRINTF("Error: Illegal Data Address (error code 0x02)\n");
        Modbus_SendError(buffer[0], buffer[1], ERROR_ILLEGAL_DATA_ADDRESS);
    }
}

/* Handle Modbus Read Input Registers (Function code 0x04) */
void Modbus_HandleReadInputRegister(uint8_t *buffer, uint16_t length)
{
    PRINTF("Handling Read Input Registers...\n"); // Debug print

    uint16_t startAddress = (buffer[2] << 8) | buffer[3]; // start address
    uint16_t registerCount = (buffer[4] << 8) | buffer[5]; // number of registers

    if ((startAddress + registerCount) <= INPUT_REGISTER_COUNT)
    {
        // Map resistance values to input registers
        for (int i = 0; i < MAX_STRIP_COUNT; i++) {
            inputRegisters[i] = resistance_array[i][0]; // Resistance value
        }

        uint8_t responseLength = 3 + registerCount * 2 + 2; // Address + Function Code + Byte Count + Data + CRC
        uint8_t txBuffer[responseLength]; // Declare txBuffer with appropriate size

        txBuffer[0] = buffer[0]; // Address
        txBuffer[1] = buffer[1]; // Function Code
        txBuffer[2] = registerCount * 2; // Byte count

        for (uint16_t i = 0; i < registerCount; i++)
        {
            txBuffer[3 + i * 2] = (inputRegisters[startAddress + i] >> 8) & 0xFF;  // High data byte
            txBuffer[4 + i * 2] = inputRegisters[startAddress + i] & 0xFF;         // Low data byte
        }

        uint16_t crc = Modbus_CRC16(txBuffer, 3 + registerCount * 2);
        txBuffer[3 + registerCount * 2] = crc & 0xFF;   // low byte CRC16
        txBuffer[4 + registerCount * 2] = (crc >> 8) & 0xFF;    // high byte CRC16

        LPUART_WriteBlocking(MODBUS_LPUART, txBuffer, responseLength);
    }

    else
    {
        //PRINTF("Error: Illegal Data Address (error code 0x02)\n");
        Modbus_SendError(buffer[0], buffer[1], ERROR_ILLEGAL_DATA_ADDRESS);
    }
}

/* Handle Modbus Write Single Register (Function code 0x06) */
void Modbus_HandleWriteSingleRegister(uint8_t *buffer, uint16_t length)
{
    PRINTF("Handling Write Single Register\n"); // Debug print

    if (length < 8) // Check for minimum frame length
    {
        return;
    }

    uint16_t address = (buffer[2] << 8) | buffer[3];
    uint16_t value = (buffer[4] << 8) | buffer[5];

    if (address < HOLDING_REGISTER_COUNT)
    {
        holdingRegisters[address] = value;

        uint16_t temp_params[7];
        memcpy(temp_params, holdingRegisters, sizeof(temp_params));
        Modbus_UpdateParameters(temp_params); // Update parameters

        uint8_t txBuffer[8];
        memcpy(txBuffer, buffer, 6);

        uint16_t crc = Modbus_CRC16(txBuffer, 6);
        txBuffer[6] = crc & 0xFF;
        txBuffer[7] = (crc >> 8) & 0xFF;

        LPUART_WriteBlocking(MODBUS_LPUART, txBuffer, 8); // Echo back the request
        return;
    }
    else
    {
        Modbus_SendError(buffer[0], buffer[1], ERROR_ILLEGAL_DATA_ADDRESS); 
        return;
    }
}

/* Handle Modbus Write Multiple Registers (Function code 0x06) */
void Modbus_HandleWriteMultipleRegisters(uint8_t *buffer, uint16_t length)
{
    PRINTF("Handling Write Multiple Registers\n"); // Debug print

    if (length < 9) // Check for minimum frame length
    {
        PRINTF("Error: Frame too short\n"); // Debug print
        return;
    }

    uint16_t startAddress = (buffer[2] << 8) | buffer[3];
    uint16_t registerCount = (buffer[4] << 8) | buffer[5];
    uint8_t byteCount = buffer[6];

    // Check for allowable data addresses
    if (registerCount < 1 || registerCount > 123 || byteCount != registerCount * 2)
    {
        Modbus_SendError(buffer[0], buffer[1], ERROR_ILLEGAL_DATA_ADDRESS);  // Data address out of possible range
        return;
    }

    // Check if number of registers to be written is in allowable range
    if ((startAddress + registerCount) <= HOLDING_REGISTER_COUNT)
    {
        for (uint16_t i = 0; i < registerCount; i++)
        {
            uint16_t value = (buffer[7 + i * 2] << 8) | buffer[8 + i * 2];   // lower and upper data byte
            holdingRegisters[startAddress + i] = value;                      // write updated parameter to holding register
        }

        uint16_t temp_params[7];
        memcpy(temp_params, holdingRegisters, sizeof(temp_params));
        Modbus_UpdateParameters(temp_params); // Update parameters

        // construct response
        uint8_t response[8];
        response[0] = buffer[0];
        response[1] = buffer[1];
        response[2] = buffer[2];
        response[3] = buffer[3];
        response[4] = buffer[4];
        response[5] = buffer[5];

        uint16_t crc = Modbus_CRC16(response, 6);
        response[6] = crc & 0xFF;
        response[7] = (crc >> 8) & 0xFF;
        LPUART_WriteBlocking(MODBUS_LPUART, response, sizeof(response)); // Echo back request with new CRC
    }
    else
    {
        Modbus_SendError(buffer[0], buffer[1], ERROR_ILLEGAL_DATA_ADDRESS);
    }
}

/* Update received system parameters */
void Modbus_UpdateParameters(uint16_t *buffer)
{
    // Set wavetype --> 1=sine, 2=square, 3=triangle
    if (buffer[0] != 0)
    {
        waveType = buffer[0] - 1;
        PRINTF("New wave type: %d, %d\n", buffer[0], waveType);
    }

    // Set Duty cycle
    if (buffer[1] != 0)
    {
        dutyCycle = buffer[1];
        PRINTF("New duty cycle: %d, %d\n", buffer[1], dutyCycle);
    }
    
    // Set wave frequency (mHz)
    if (buffer[2] != 0)
    {
        desiredFrequency = (float)buffer[2] / 1000.0f;
        PRINTF("New wave freq: %d, %.3f\n", buffer[2], desiredFrequency);
    }

    // Set wave amplitude (mV)
    if (buffer[3] != 0)
    {
        desiredAmplitude = (float)buffer[3] / 1000.0f;
        PRINTF("New amplitude: %d, %.3f\n", buffer[3], desiredAmplitude);
    }

    // Set excitation voltage per strip (mV)
    if (buffer[4] != 0)
    {
        g_excitation_voltage_per_resistor = (float)buffer[4] / 1000.0f;
        PRINTF("New excitation V: %d, %.3f\n", buffer[4], g_excitation_voltage_per_resistor);
    }

    // Set MUX frequency 
    if (buffer[5] != 0)
    {
        MUX_freq = buffer[5];
        PRINTF("New MUX freq: %d, %d\n", buffer[5], MUX_freq);
    }

    // Set active strips
    if (buffer[6] != 0)
    {
        g_strip_count = 0;
        for (int i = 0; i < MAX_STRIP_COUNT; i++) 
        {
            if ((buffer[6] & (1 << i)) == 1){
                active_strips[i] = true;
                g_strip_count++;
            }
            else{
                active_strips[i] = false;
            }
            PRINTF("Strip %d: %d\n", i, active_strips[i]);
        }
    }
    ModbusParamChanged = true;
}



/* Check for illegal parameters, returns false if illegal parameters have been found*/
bool Modbus_DataErrorPresent(uint16_t *buffer)
{
    if (buffer[0] > 3 ||        // Wave type: 1, 2, 3
        buffer[1] > 100 ||      // 0 < Duty cycle < 100
        buffer[2] < 2 ||        
        buffer[2] > 500 ||        // 2 < wave frequency (mHz) < 500
        buffer[3] > 23000 ||       // 0 < wave amplitude (mV) < 23000 
        buffer[4] < 500 ||      
        buffer[4] > 1000 ||     // 500 < excitation voltage per strip (mV) < 1000
        buffer[5] > 8 ||        // 0 < mux frequency (Hz) < 8
        buffer[6] > (uint16_t)pow(2, MAX_STRIP_COUNT))  // 8 < # of strips 
    {
        return true;
    }
    return false;
}

/* Send Modbus error response */
void Modbus_SendError(uint8_t address, uint8_t function, uint8_t errorCode)
{
    uint8_t frame[5];
    frame[0] = address;
    frame[1] = function | 0x80; // Set MSB to indicate error
    frame[2] = errorCode;

    uint16_t crc = Modbus_CRC16(frame, 3);
    frame[3] = crc & 0xFF;
    frame[4] = (crc >> 8) & 0xFF;

    LPUART_WriteBlocking(MODBUS_LPUART, frame, 5);
}