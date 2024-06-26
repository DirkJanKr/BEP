import threading
import serial
import datetime
import pandas as pd
import csv
import re
import time
from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QFrame, QLineEdit, QComboBox, QFormLayout, QPushButton, QGridLayout, QTextEdit, QMessageBox, QSizePolicy, QCheckBox, QFileDialog, QDialog, QDialogButtonBox
from PySide6.QtCore import Qt, Signal, Slot, QObject, QTimer
from PySide6.QtGui import QPixmap
import matplotlib.pyplot as plt
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas


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
                    response = self.ser.readline().decode(errors='ignore').strip()  # Handle decoding errors
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
        print(f"Sending parameters to microcontroller: {parameters}")
        self.ser.write(parameters.encode())

class PlotCanvas(FigureCanvas):
    def __init__(self, parent=None):
        self.fig, self.ax = plt.subplots()
        super().__init__(self.fig)
        self.setParent(parent)
        self.ax.set_title('Real-time Resistance Plot')
        self.ax.set_xlabel('Time (s)')
        self.ax.set_ylabel('Resistance (Ohms)')
        self.ax.grid(True)
        self.strip_visibility = [True] * 8  # Visibility for each strip

    def load_data(self):
        try:
            df = pd.read_csv('source/GUI/resistance_data.csv')
            if df.empty:
                print("The CSV file is empty.")
            return df
        except FileNotFoundError:
            print("The CSV file was not found.")
            return pd.DataFrame(columns=['Strip', 'Time (s)', 'Resistance (Ohms)'])
        except pd.errors.EmptyDataError:
            print("The CSV file is empty.")
            return pd.DataFrame(columns=['Strip', 'Time (s)', 'Resistance (Ohms)'])

    def update_plot(self):
        self.ax.clear()
        df = self.load_data()
        if not df.empty:
            for strip in df['Strip'].unique():
                if self.strip_visibility[strip - 1]:  # Check if the strip should be visible
                    strip_data = df[df['Strip'] == strip]
                    self.ax.plot(strip_data['Time (s)'], strip_data['Resistance (Ohms)'], label=f'Strip {strip}')
            self.ax.legend(loc='upper right')
        else:
            print("No data available for plotting.")
        self.ax.set_title('Real-time Resistance Plot')
        self.ax.set_xlabel('Time (s)')
        self.ax.set_ylabel('Resistance (Ohms)')
        self.ax.grid(True)
        self.draw()

    def toggle_strip_visibility(self, strip_index):
        self.strip_visibility[strip_index] = not self.strip_visibility[strip_index]
        self.update_plot()

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Styled GUI")
        self.showMaximized()
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

        # Setup a timer to update the plot
        self.plot_update_timer = QTimer()
        self.plot_update_timer.timeout.connect(self.update_plot_canvas)

        # Initialize CSV file
        self.csv_file_path = 'source/GUI/resistance_data.csv'
        self.init_csv_file()

        # Measurement started flag
        self.measurement_started = False

    def init_csv_file(self):
        try:
            with open(self.csv_file_path, 'r') as csvfile:
                pass
        except FileNotFoundError:
            with open(self.csv_file_path, 'w', newline='') as csvfile:
                csv_writer = csv.writer(csvfile)
                current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                csv_writer.writerow(['Strip', 'Time (s)', 'Resistance (Ohms)', f'Date and time measurement started: {current_time}'])
        self.csv_file = open(self.csv_file_path, mode='a', newline='')
        self.csv_writer = csv.writer(self.csv_file)

    def clear_csv_file(self):
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        with open(self.csv_file_path, 'w', newline='') as csvfile:
            csv_writer = csv.writer(csvfile)
            csv_writer.writerow(['Strip', 'Time (s)', 'Resistance (Ohms)', f'Date and time measurement started: {current_time}'])
        self.csv_file = open(self.csv_file_path, mode='a', newline='')
        self.csv_writer = csv.writer(self.csv_file)


# def clear_csv_file(self):
#     current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
#     header_row = ['Strip', 'Time (s)', 'Resistance (Ohms)', current_time]

#     # Write the header row to the CSV file
#     with open(self.csv_file_path, 'w', newline='') as csv_file:
#         csv_writer = csv.writer(csv_file)
#         csv_writer.writerow(header_row)


    def download_csv_file(self):
        file_dialog = QFileDialog()
        file_dialog.setAcceptMode(QFileDialog.AcceptSave)
        file_dialog.setNameFilter("CSV Files (*.csv)")
        file_dialog.setDefaultSuffix("csv")

        if file_dialog.exec():
            file_path = file_dialog.selectedFiles()[0]
            try:
                with open(self.csv_file_path, 'r') as source_file:
                    with open(file_path, 'w') as dest_file:
                        dest_file.write(source_file.read())
                self.show_success_message(f"File saved to {file_path}")

                # QMessageBox.information(self, "Success", f"File saved to {file_path}")
            except Exception as e:
                QMessageBox.critical(self, "Error", f"Failed to save file: {e}")


    def create_top_left_frame(self):
        top_left_frame = QFrame()
        top_left_frame.setObjectName("topLeftFrame")
        top_left_frame.setStyleSheet("QFrame#topLeftFrame { border: 2px solid grey; border-radius: 5px; }")  # Add border to top left frame

        # Add the logo to the top left frame
        logo_label = QLabel()
        logo_label.setObjectName("logoLabel")
        pixmap = QPixmap("source/GUI/TUDelft-logo_full-color.png")  # Replace with the actual path to your logo
        logo_label.setPixmap(pixmap.scaled(250, 125, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        logo_label.setAlignment(Qt.AlignTop | Qt.AlignHCenter)
        logo_label.setStyleSheet("QLabel#logoLabel { border: none; }")  # Remove border from logo label

        # Serial port input field and connect button
        serial_port_label = QLabel("Serial Port:")
        serial_port_label.setAlignment(Qt.AlignCenter)
        serial_port_label.setStyleSheet("QLabel { border: none; }")  # Remove border from label
        self.serial_port_input = QLineEdit()
        self.serial_port_input.setPlaceholderText("Enter serial port (e.g., COM12 or /dev/ttyUSB0)")
        self.serial_port_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        
        connect_button = QPushButton("Connect")
        connect_button.setObjectName("connectButton")  # Set an object name for styling
        connect_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        connect_button.setMinimumHeight(20)
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
        for i in range(8):
            button = QPushButton(f"Strip {i + 1}")
            button.setCheckable(True)
            button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
            button.setMinimumHeight(20)
            button.clicked.connect(lambda _, x=i: self.toggle_strip(x))
            self.strip_buttons.append(button)
            strip_buttons_layout.addWidget(button, i // 4, i % 4)

        # Create the container for input fields
        input_container = QFrame()
        input_container.setStyleSheet("QFrame { border: none; }")  # Remove border from input container
        input_layout = QFormLayout(input_container)
        input_layout.setContentsMargins(0, 0, 0, 0)
        input_layout.setSpacing(5)

        # Excitation Voltage per strip
        excitation_voltage_label = QLabel("Excitation Voltage per Strip:")
        excitation_voltage_label.setObjectName("inputLabel")  #Set a common object name for stylesheet
        self.excitation_voltage_input = QLineEdit()
        self.excitation_voltage_input.setPlaceholderText("Enter Excitation Voltage")
        self.excitation_voltage_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(excitation_voltage_label, self.excitation_voltage_input)

        # MUX Frequency
        mux_freq_label = QLabel("MUX Frequency:")
        mux_freq_label.setObjectName("inputLabel")  # Set a common object name for stylesheet
        self.mux_freq_input = QLineEdit()
        self.mux_freq_input.setPlaceholderText("Enter MUX Frequency")
        self.mux_freq_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(mux_freq_label, self.mux_freq_input)

        # Wave Frequency
        wave_freq_label = QLabel("Wave Frequency:")
        wave_freq_label.setObjectName("inputLabel")  # Set a common object name for stylesheet
        self.wave_freq_input = QLineEdit()
        self.wave_freq_input.setPlaceholderText("Enter Wave Frequency")
        self.wave_freq_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(wave_freq_label, self.wave_freq_input)

        # Wave Type
        wave_type_label = QLabel("Wave Type:")
        wave_type_label.setObjectName("inputLabel")  # Set a common object name for stylesheet
        self.wave_type_dropdown = QComboBox()
        self.wave_type_dropdown.addItem("Sine wave")  # Add entry to dropdown
        self.wave_type_dropdown.addItem("Square wave")  # Add entry to dropdown
        self.wave_type_dropdown.addItem("Triangle wave")  # Add entry to dropdown
        self.wave_type_dropdown.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(wave_type_label, self.wave_type_dropdown)

        # Wave Amplitude
        wave_amp_label = QLabel("Wave Amplitude:")
        wave_amp_label.setObjectName("inputLabel")  # CHANGED: Set a common object name for stylesheet
        self.wave_amp_input = QLineEdit()
        self.wave_amp_input.setPlaceholderText("Enter Wave Amplitude")
        self.wave_amp_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(wave_amp_label, self.wave_amp_input)

        # Duty Cycle
        duty_cycle_label = QLabel("Duty Cycle:")
        duty_cycle_label.setObjectName("inputLabel")  # CHANGED: Set a common object name for stylesheet
        self.duty_cycle_input = QLineEdit()
        self.duty_cycle_input.setPlaceholderText("Enter Duty Cycle")
        self.duty_cycle_input.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        input_layout.addRow(duty_cycle_label, self.duty_cycle_input)

        # Create a container for buttons with a vertical layout to stack them
        button_container = QFrame()
        button_container.setStyleSheet("QFrame { border: none; }")  # Remove border from button container
        button_layout = QVBoxLayout(button_container)
        button_layout.setAlignment(Qt.AlignCenter)  # Center-align the buttons

        # Button to set the parameters and start measurement
        start_button = QPushButton("Start Measurement")
        start_button.setObjectName("startButton")  # Set an object name for styling
        start_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        start_button.setMinimumHeight(20)
        start_button.clicked.connect(self.start_measurement)  # Connect to the start_measurement method
        button_layout.addWidget(start_button)

        # Button to download data as CSV file
        download_button = QPushButton("Download Data")
        download_button.setObjectName("downloadButton")  # Set an object name for styling
        download_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        download_button.setMinimumHeight(20)
        button_layout.addWidget(download_button)
        download_button.clicked.connect(self.download_csv_file)

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
        self.heater_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)  # Use size policy instead of fixed size
        self.heater_button.setMinimumHeight(20)
        self.heater_button.setStyleSheet("background-color: red;")
        self.heater_button.clicked.connect(self.toggle_heater)
        heater_layout.addWidget(manual_heater_control_label)
        heater_layout.addWidget(self.heater_button)

        # Main layout for top left frame
        top_left_frame_layout = QVBoxLayout(top_left_frame)
        top_left_frame_layout.setContentsMargins(5, 5, 5, 5)  # Set margins
        top_left_frame_layout.setSpacing(0)  # Set spacing between elements
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
            self.serial_thread.new_data.connect(self.write_to_csv)  # Connect to the CSV writing method
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
        msg_box.exec()

    def show_success_message(self, message):
        msg_box = QMessageBox()
        msg_box.setIcon(QMessageBox.Information)
        msg_box.setWindowTitle("Success")
        msg_box.setText(message)
        msg_box.exec()

    def validate_excitation_voltage(self):
        try:
            value = float(self.excitation_voltage_input.text())
            if not (0.5 <= value <= 1.0):
                raise ValueError("Value out of range")
            return True
        except ValueError:
            self.show_error_message("Invalid excitation voltage. Please enter a value between 0.5 and 1.0.")
            return False

    # Check if the MUX frequency is an integer, greater than 0 and smaller than 9
    def validate_mux_frequency(self):
        try:
            value = int(self.mux_freq_input.text())
            if not (1 <= value <= 8):
                raise ValueError("Value out of range")
            return True
        except ValueError:
            self.show_error_message("Invalid MUX frequency. Please enter an integer value between 1 and 8.")
            return False

    # Check if the wave frequency of DAC hot plate is within range of 0.002 and 0.5 Hz
    def validate_wave_frequency(self):
        try:
            value = float(self.wave_freq_input.text())
            if not (0.002 <= value <= 0.5):
                raise ValueError("Value out of range")
            return True
        except ValueError:
            self.show_error_message("Invalid wave frequency. Please enter a value between 0.002 and 0.5 Hz.")
            return False

    # Check if the DAC hot plate voltage is within range of 0 to 23V    
    def validate_dac_hot_plate_voltage(self):
        try:
            value = float(self.wave_amp_input.text())
            if not (0 <= value <= 23):
                raise ValueError("Value out of range")
            return True
        except ValueError:
            self.show_error_message("Invalid DAC hot plate voltage. Please enter a value between 0 and 23V.")
            return False

    # Check if the duty cycle is an integer and between and including 0 to 100
    def validate_duty_cycle(self):
        try:
            value = int(self.duty_cycle_input.text())
            if not (0 <= value <= 100):
                raise ValueError("Value out of range")
            return True
        except ValueError:
            self.show_error_message("Invalid duty cycle. Please enter an integer value between 0 and 100.")
            return False

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
        if not self.show_confirmation_dialog():
            return  # Exit if user cancels the action

        # Check if measurement already started
        if self.measurement_started:
            self.clear_csv_file()  # Clear the CSV file for a new measurement

        if not self.validate_excitation_voltage():
            return
        if not self.validate_mux_frequency():
            return
        if not self.validate_wave_frequency():
            return
        if not self.validate_dac_hot_plate_voltage():
            return
        if not self.validate_duty_cycle():
            return

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

            # Clear the CSV file and write headers
            self.clear_csv_file()

            # Start the plot update timer
            self.measurement_started = True
            self.plot_update_timer.start(10000)  # Update every 10 seconds

        else:
            self.show_error_message("Serial port not connected.")

    def show_confirmation_dialog(self):
        dialog = QDialog(self)
        dialog.setWindowTitle("Confirm Action")

        # Create dialog layout
        dialog_layout = QVBoxLayout()

        # Add message to dialog
        message_label = QLabel("Starting a new measurement deletes the data from the previous measurement.\nDo you want to continue?")
        message_label.setStyleSheet("QLabel { border: none; font-size: 14px; }")  # Remove border and set font size
        dialog_layout.addWidget(message_label)

        # Add buttons to dialog
        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        dialog_layout.addWidget(buttons)

        # Connect buttons
        buttons.accepted.connect(dialog.accept)
        buttons.rejected.connect(dialog.reject)

        dialog.setLayout(dialog_layout)

        # Show dialog and get the result
        result = dialog.exec_()

        return result == QDialog.Accepted

    @Slot(str)
    def update_serial_monitor(self, data):
        self.serial_monitor.append(data)  # Append new data to the serial monitor

    @Slot(str)
    def write_to_csv(self, data):
        match = re.match(r"Resistance of strip (\d+): (\d+), Time: (\d+)", data)
        if match:
            strip = int(match.group(1))
            resistance = int(match.group(2))
            time_val = int(match.group(3)) / 1000  # Convert time to seconds
            self.csv_writer.writerow([strip, time_val, resistance])
            self.csv_file.flush()  # Ensure the data is written to disk immediately

    def update_plot_canvas(self):
        if self.measurement_started:
            self.plot_canvas.update_plot()

    def create_top_right_frame(self):
        top_right_frame = QFrame()
        top_right_frame.setObjectName("topRightFrame")
        top_right_frame.setStyleSheet("QFrame#topRightFrame { border: 2px solid grey; border-radius: 5px; }")  # Add outer border to top right frame

        self.plot_canvas = PlotCanvas(top_right_frame)

        top_right_frame_layout = QVBoxLayout(top_right_frame)
        top_right_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        top_right_frame_layout.setSpacing(20)  # Set spacing between elements
        top_right_frame_layout.addWidget(self.plot_canvas)

        # Add strip visibility buttons
        strip_visibility_layout = QHBoxLayout()
        for i in range(8):
            checkbox = QCheckBox(f"Strip {i + 1}")
            checkbox.setChecked(True)
            checkbox.stateChanged.connect(lambda state, x=i: self.plot_canvas.toggle_strip_visibility(x))
            strip_visibility_layout.addWidget(checkbox)

        top_right_frame_layout.addLayout(strip_visibility_layout)
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