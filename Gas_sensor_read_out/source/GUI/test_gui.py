import threading
from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QFrame, QLineEdit, QComboBox, QFormLayout, QPushButton, QGridLayout, QTextEdit, QMessageBox
from PySide6.QtCore import Qt, Signal, Slot, QObject
from PySide6.QtGui import QPixmap
import serial
import time

class SerialThread(QObject):
    new_data = Signal(str)  # Signal to send new data to the GUI

    def __init__(self, port):
        super().__init__()
        self.flag = True
        self.ser = None
        try:
            self.ser = serial.Serial(port, 115200)
            time.sleep(2)  # Wait for the connection to establish
        except serial.SerialException as e:
            print(f"Failed to open serial port: {e}")

    def run(self):
        if self.ser is None:
            return
        while self.flag:
            try:
                if self.ser.in_waiting > 0:
                    response = self.ser.readline().decode().strip()
                    self.new_data.emit(response)  # Emit signal with new data
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                self.flag = False

    def stop(self):
        self.flag = False
        if self.ser:
            self.ser.close()

    def send_parameters(self, waveType, dutyCycle, desiredFrequency, desiredAmplitude, excitationVoltagePerStrip, muxFrequency, strip_selection):
        if self.ser is None:
            return
        parameters = f"{waveType},{dutyCycle},{desiredFrequency},{desiredAmplitude},{excitationVoltagePerStrip},{muxFrequency},{strip_selection}\n"
        self.ser.write(parameters.encode())

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Styled GUI")
        self.setGeometry(10, 10, 720, 480)

        # Load and apply the stylesheet
        with open("source/GUI/styles.qss", "r") as f:
            self.setStyleSheet(f.read())

        # Main widget
        main_widget = QWidget()
        self.setCentralWidget(main_widget)

        # Main layout
        main_layout = QVBoxLayout(main_widget)

        # Create and add frames
        top_left_frame = self.create_top_left_frame()
        top_right_frame = self.create_top_right_frame()
        bottom_frame = self.create_bottom_frame()

        # Top layout (containing two frames)
        top_layout = QHBoxLayout()
        top_layout.addWidget(top_left_frame, 1)  # 1/3 of the width
        top_layout.addWidget(top_right_frame, 2)  # 2/3 of the width
        main_layout.addLayout(top_layout, 3)  # 3/4 of the height

        # Add bottom frame
        main_layout.addWidget(bottom_frame, 1)  # 1/4 of the height

        # Serial communication setup
        self.serial_thread = None
        self.thread = None
        self.strip_selection = [0] * 8  # Initialize strip selection list

    def create_top_left_frame(self):
        top_left_frame = QFrame()
        top_left_frame.setObjectName("topLeftFrame")
        top_left_frame.setStyleSheet("QFrame#topLeftFrame { border: 2px solid grey; border-radius: 5px; }")  # Add border to top left frame

        # Add the logo to the top left frame
        logo_label = QLabel()
        logo_label.setObjectName("logoLabel")
        pixmap = QPixmap("source/GUI/TUDelft-logo_full-color.png")  # Replace with the actual path to your logo
        logo_label.setPixmap(pixmap.scaled(300, 150, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        logo_label.setAlignment(Qt.AlignTop | Qt.AlignHCenter)
        logo_label.setStyleSheet("QLabel#logoLabel { border: none; }")  # Remove border from logo label

        # Serial port input field and connect button
        serial_port_label = QLabel("Serial Port:")
        serial_port_label.setAlignment(Qt.AlignCenter)
        serial_port_label.setStyleSheet("QLabel { border: none; }")  # Remove border from label
        self.serial_port_input = QLineEdit()
        self.serial_port_input.setPlaceholderText("Enter serial port (e.g., COM12 or /dev/ttyUSB0)")
        self.serial_port_input.setFixedWidth(200)
        
        connect_button = QPushButton("Connect")
        connect_button.setObjectName("connectButton")  # Set an object name for styling
        connect_button.setFixedSize(200, 40)  # Set button size
        connect_button.clicked.connect(self.connect_serial_port)  # Connect to the connect_serial_port method

        # Layout for serial port input and connect button
        serial_layout = QVBoxLayout()
        serial_layout.setAlignment(Qt.AlignCenter)
        serial_layout.addWidget(serial_port_label)
        serial_layout.addWidget(self.serial_port_input)
        serial_layout.addWidget(connect_button)

        # Create a label for "Select active strips"
        select_active_strips_label = QLabel("Select active strips")
        select_active_strips_label.setAlignment(Qt.AlignCenter)
        select_active_strips_label.setStyleSheet("QLabel { border: none; }")  # Remove border from label

        # Create a container for buttons to select active graphene strips without a border
        strip_buttons_frame = QFrame()
        strip_buttons_frame.setStyleSheet("QFrame { border: none; }")  # Remove border from strip buttons container
        strip_buttons_layout = QGridLayout(strip_buttons_frame)
        strip_buttons_layout.setAlignment(Qt.AlignCenter)

        self.strip_buttons = []
        fixed_width = 200
        for i in range(8):
            button = QPushButton(f"Strip {i + 1}")
            button.setCheckable(True)
            button.setFixedSize(fixed_width / 2, 40)
            button.clicked.connect(lambda _, x=i: self.toggle_strip(x))
            self.strip_buttons.append(button)
            strip_buttons_layout.addWidget(button, i // 4, i % 4)

        # Create the container for input fields
        input_container = QFrame()
        input_container.setStyleSheet("QFrame { border: none; }")  # Remove border from input container
        input_layout = QFormLayout(input_container)
        input_layout.setContentsMargins(0, 0, 0, 0)
        input_layout.setSpacing(10)

        # Excitation Voltage per strip
        self.excitation_voltage_input = QLineEdit()
        self.excitation_voltage_input.setPlaceholderText("Enter Excitation Voltage")
        self.excitation_voltage_input.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("Excitation Voltage per Strip:"), self.excitation_voltage_input)

        # MUX Frequency
        self.mux_freq_input = QLineEdit()
        self.mux_freq_input.setPlaceholderText("Enter MUX Frequency")
        self.mux_freq_input.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("MUX Frequency:"), self.mux_freq_input)

        # Wave Frequency
        self.wave_freq_input = QLineEdit()
        self.wave_freq_input.setPlaceholderText("Enter Wave Frequency")
        self.wave_freq_input.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("Wave Frequency:"), self.wave_freq_input)

        # Wave Type
        self.wave_type_dropdown = QComboBox()
        self.wave_type_dropdown.addItem("Sine wave")  # Add entry to dropdown
        self.wave_type_dropdown.addItem("Square wave")  # Add entry to dropdown
        self.wave_type_dropdown.addItem("Triangle wave")  # Add entry to dropdown
        self.wave_type_dropdown.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("Wave Type:"), self.wave_type_dropdown)

        # Wave Amplitude
        self.wave_amp_input = QLineEdit()
        self.wave_amp_input.setPlaceholderText("Enter Wave Amplitude")
        self.wave_amp_input.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("Wave Amplitude:"), self.wave_amp_input)

        # Duty Cycle
        self.duty_cycle_input = QLineEdit()
        self.duty_cycle_input.setPlaceholderText("Enter Duty Cycle")
        self.duty_cycle_input.setFixedWidth(fixed_width)
        input_layout.addRow(QLabel("Duty Cycle:"), self.duty_cycle_input)        

        # Create a container for buttons with a vertical layout to stack them
        button_container = QFrame()
        button_container.setStyleSheet("QFrame { border: none; }")  # Remove border from button container
        button_layout = QVBoxLayout(button_container)
        button_layout.setAlignment(Qt.AlignCenter)  # Center-align the buttons

        # Button to set the parameters and start measurement
        start_button = QPushButton("Start Measurement")
        start_button.setObjectName("startButton")  # Set an object name for styling
        start_button.setFixedSize(fixed_width, 40)  # Set button size
        start_button.clicked.connect(self.start_measurement)  # Connect to the start_measurement method
        button_layout.addWidget(start_button)

        # Button to download data as CSV file
        download_button = QPushButton("Download Data")
        download_button.setObjectName("downloadButton")  # Set an object name for styling
        download_button.setFixedSize(fixed_width, 40)  # Set button size
        button_layout.addWidget(download_button)

        # Create a container for the manual heater control label and button
        heater_container = QFrame()
        heater_container.setStyleSheet("QFrame { border: none; }")  # Remove border from heater container
        heater_layout = QVBoxLayout(heater_container)
        heater_layout.setAlignment(Qt.AlignCenter)  # Center-align the elements

        # Create a label for "Manual heater control"
        manual_heater_control_label = QLabel("Manual heater control")
        manual_heater_control_label.setAlignment(Qt.AlignCenter)
        manual_heater_control_label.setStyleSheet("QLabel { border: none; }")  # Remove border from label

        # Button to toggle heater
        self.heater_button = QPushButton("Current state: heater off")
        self.heater_button.setObjectName("heaterButton")
        self.heater_button.setFixedSize(fixed_width, 40)  # Set button size
        self.heater_button.setStyleSheet("background-color: red;")
        self.heater_button.clicked.connect(self.toggle_heater)
        heater_layout.addWidget(manual_heater_control_label)
        heater_layout.addWidget(self.heater_button)

        # Main layout for top left frame
        top_left_frame_layout = QVBoxLayout(top_left_frame)
        top_left_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        top_left_frame_layout.setSpacing(20)  # Set spacing between elements
        top_left_frame_layout.addWidget(logo_label)
        top_left_frame_layout.addLayout(serial_layout)  # Add the serial port layout
        top_left_frame_layout.addWidget(select_active_strips_label)  # Add the label for selecting active strips
        top_left_frame_layout.addWidget(strip_buttons_frame)  # Add the strip buttons container
        top_left_frame_layout.addWidget(input_container)  # Add the input container
        top_left_frame_layout.addWidget(heater_container)  # Add the heater container
        top_left_frame_layout.addWidget(button_container)  # Add the button container

        return top_left_frame

    def connect_serial_port(self):
        port = self.serial_port_input.text()
        if port:
            self.serial_thread = SerialThread(port)
            self.serial_thread.new_data.connect(self.update_serial_monitor)
            self.thread = threading.Thread(target=self.serial_thread.run)
            self.thread.start()
            # Check if the serial port was successfully opened
            if self.serial_thread.ser is None:
                self.show_error_message(f"Failed to connect to serial port: {port}")
            else:
                self.show_success_message(f"Connected to serial port: {port}")
        else:
            self.show_error_message("Please enter a serial port.")

    def show_error_message(self, message):
        msg_box = QMessageBox()
        msg_box.setIcon(QMessageBox.Critical)
        msg_box.setWindowTitle("Error")
        msg_box.setText(message)
        msg_box.exec_()

    def show_success_message(self, message):
        msg_box = QMessageBox()
        msg_box.setIcon(QMessageBox.Information)
        msg_box.setWindowTitle("Success")
        msg_box.setText(message)
        msg_box.exec_()

    def toggle_strip(self, index):
        button = self.strip_buttons[index]
        self.strip_selection[index] = 1 if button.isChecked() else 0
        if button.isChecked():
            button.setStyleSheet("background-color: green;")
        else:
            button.setStyleSheet("")
        print(f"Strip {index + 1} active: {button.isChecked()}")

    def toggle_heater(self):
        if self.heater_button.text() == "Current state: heater off":
            self.heater_button.setText("Current state: heater on")
            self.heater_button.setStyleSheet("background-color: green;")
        else:
            self.heater_button.setText("Current state: heater off")
            self.heater_button.setStyleSheet("background-color: red;")
        print(self.heater_button.text())

    def start_measurement(self):
        waveType = self.wave_type_dropdown.currentIndex()  # Get the wave type as an integer
        dutyCycle = int(self.duty_cycle_input.text())
        desiredFrequency = float(self.wave_freq_input.text())
        desiredAmplitude = float(self.wave_amp_input.text())
        excitationVoltagePerStrip = float(self.excitation_voltage_input.text())
        muxFrequency = float(self.mux_freq_input.text())
        strip_selection = ''.join(map(str, self.strip_selection))

        # Send parameters to the microcontroller
        if self.serial_thread and self.serial_thread.ser:
            self.serial_thread.send_parameters(waveType, dutyCycle, desiredFrequency, desiredAmplitude, excitationVoltagePerStrip, muxFrequency, strip_selection)
            print("Parameters sent to microcontroller")
        else:
            self.show_error_message("Serial port not connected.")

    @Slot(str)
    def update_serial_monitor(self, data):
        self.serial_monitor.append(data)  # Append new data to the serial monitor

    def create_top_right_frame(self):
        top_right_frame = QFrame()
        top_right_frame.setObjectName("topRightFrame")
        top_right_frame.setStyleSheet("QFrame#topRightFrame { border: 2px solid grey; border-radius: 5px; }")  # Add outer border to top right frame

        # Add example plot to the top right frame
        plot_label = QLabel()
        plot_label.setObjectName("plotLabel")
        pixmap = QPixmap("source/GUI/example_plot.png")  # Replace with the actual path to your image
        plot_label.setPixmap(pixmap.scaled(720, 480, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        plot_label.setAlignment(Qt.AlignCenter)
        plot_label.setStyleSheet("QLabel#plotLabel { border: none; }")

        top_right_frame_layout = QVBoxLayout(top_right_frame)
        top_right_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        top_right_frame_layout.setSpacing(20)  # Set spacing between elements
        top_right_frame_layout.addWidget(plot_label)

        return top_right_frame

    def create_bottom_frame(self):
        bottom_frame = QFrame()
        bottom_frame.setObjectName("bottomFrame")
        bottom_frame.setStyleSheet("QFrame#bottomFrame { border: 2px solid grey; border-radius: 5px; }")  # Add outer border to bottom frame

        self.serial_monitor = QTextEdit()
        self.serial_monitor.setReadOnly(True)
        self.serial_monitor.setStyleSheet("background-color: black; color: white;")

        bottom_frame_layout = QVBoxLayout(bottom_frame)
        bottom_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        bottom_frame_layout.setSpacing(20)  # Set spacing between elements
        bottom_frame_layout.addWidget(self.serial_monitor)

        return bottom_frame

    def closeEvent(self, event):
        if self.serial_thread:
            self.serial_thread.stop()
            self.thread.join()
        super().closeEvent(event)

# Create the application
app = QApplication([])

# Create and show the main window
window = MainWindow()
window.show()

# Run the application's event loop
app.exec()
