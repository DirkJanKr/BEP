import serial
import time

# Serial port configuration
SERIAL_PORT = '/dev/tty.usbmodem3QGPWXPFGTOFX3'  # Update with the appropriate serial port on your system
BAUD_RATE = 115200
TIMEOUT = 10

# Open serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

def send_command(command):
    print(f"Sending command: {command.strip()}")
    ser.write(command.encode())
    time.sleep(1)  # Give some time for the microcontroller to process and respond
    response = ser.readline().decode('utf-8', errors='replace').strip()
    print(f"Received response: {response}")
    return response

def set_parameters(freq, amplitude, duty_cycle):
    command = f"SET_PARAMS {freq} {amplitude} {duty_cycle}\n"
    response = send_command(command)
    print("Response:", response)

if __name__ == "__main__":
    # Example: Set parameters
    frequency = 1  # Hz
    amplitude = 1.0   # V
    duty_cycle = 50   # %

    set_parameters(frequency, amplitude, duty_cycle)

    # Close serial port
    ser.close()
