// global.c
#include "global.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_Dac14InputIndex = 0U;
uint32_t g_Dac14Values[MAX_ARRAY_SIZE];
uint32_t g_Dac14ValueArraySize;
uint32_t waveType = 0;  // 0: sine, 1: square, 2: triangle (or sawtooth depending on the duty cycle)
uint32_t dutyCycle = 50;    // in percentage
float desiredFrequency = 0.05;  // in Hz
uint32_t desiredAmplitude = 21;  // in volts

// flag to indicate g_strip array is filled
volatile bool I_sens_strip_values_ready = false;
volatile bool V_sens_strip_values_ready = false;


// Declare the ADC value variable
volatile uint16_t I_sens_AdcValue = 0;
volatile uint16_t V_sens_AdcValue = 0;


// Variable to keep track of the number of graphene strips to be read
uint8_t g_strip_count = 0;

// 2D Array to store the read values from the ADC, corresponding strip number, and in the future the timestamp
uint32_t I_sens_strip_values[8][2];
uint32_t V_sens_strip_values[8][2];

int resistance_array[8][2] = {0};

// Variable to keep track of the timestamp in ms
volatile uint32_t g_timestamp_ms = 0;

/*MUX parameters*/
uint8_t MUX_enable = 0;
uint8_t MUX_channel;
uint16_t MUX_freq = 8;

/*DAC for the excitation voltage*/
bool g_excitation_voltage_on = true;
float g_excitation_voltage_per_resistor = 1.0; // in volts;
uint8_t g_resistor_formation = 8;

bool active_strips[8] = {false, false, false, false, false, false, false, false};

// Value to store the result of the curr strip
uint32_t current_adc_result = 0;

// flag to indicate if parameters are changed through modbus
bool ModbusParamChanged = false;

// flag set to true if modbus is used instead of the GUI
bool ModbusFlag = false;








