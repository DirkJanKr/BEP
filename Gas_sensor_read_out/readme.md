============================================
Graphene Strip Resistance Measurement System
============================================

Overview
========

This project involves the development of a high-resolution resistance measurement system for graphene strips arrangerd in a daisy chain configuration.
The system is designed to interface with both a Python-based GUI and Modbus interface.
The graphene strips can be measured in any configuration upto 8 graphene strips. 


Features 
===========
- High-Resolution ADCs: Utilizes the microcontroller's 16-bit ADCs with a voltage step of 0.02 mV to ensure precise resistance measurements.
- Multiplexer (MUX): Supports up to 8 different channels for cycling through multiple graphene strips.
- Synchronous Measurement: Uses a timer to synchronize the ADCs and the MUX, ensuring accurate and consistent measurements.
- Averaging Strategy: Implements both hardware averaging and software averaging to improve measurement accuracy.
- Power MUX Integration: Integrates TPS2121 power multiplexer to switch between 5V and 3.3V power sources based on the selected interface.
- Dual Interface: Supports both a Python GUI and a Modbus interface for data display and control.

Files
==========
- main.c: Main application code.
- adc_16_strip.c/h: ADC and MUX control.
- gpio_for_mux.c/h: GPIO control for the MUX.
- DAC_micro_hot_plate.c/h: DAC control for micro hotplate.
- DAC_excitation_voltage.c/h: DAC control for excitation voltage.
- modbus.c/h: Modbus interface handling.
- test_gui.py: Python interface.
- power_mux.c/h: Power MUX control.
- global.c/h: Global definitions and variables
- drivers: fsl files, pin_mux.c/h and clock_config,c/h

Installations
=============
- VS Code
- Python 3.12 or higher
- MCUXpresso for VS Code extension

Virtual environment
===================
In the Pipfile all packages are defined which need to be installed. Make sure to install and use Python 3.12 as the project will not run with older versions of Python. To make this virtual environment, write the following lines in the VS code terminal:
---

SDK version
===========
- Version: 2.14.0

Toolchain supported
===================
- GCC ARM Embedded  12.2
- MCUXpresso  11.9.0

Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 Board
- Personal Computer

Board settings
==============
DAC (excitation voltage) output pin: P4_2
DAC (MHP control) output pin: ANA_6
ADC (voltage measurement) input pin: P4_23
ADC ('indirect' current measurement) input pin: P4_21
UART_RXD pin: P4_0
UART_TXD pin: P4_1
MUX (enable) pin: P4_18
MUX (MSB) pin: P4_16
MUX (...) pin: P4_14
MUX (LSB) pin: P4_12
Power MUX output pin: P2_10

Working of the project
======================



Prepare the Demo
================
1. Connect the type-c and mini USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.
5. A multimeter may be used to measure the DAC output voltage.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

DAC14 Basic Example.

Please input a value (0 - 16383) to output with DAC: 1000
Input value is 1000
DAC out: 1000

The user can measure the DAC output pin to check the output voltage.

