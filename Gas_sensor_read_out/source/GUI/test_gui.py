from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QFrame, QLineEdit, QComboBox, QFormLayout, QPushButton
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Styled GUI")
        self.setGeometry(100, 100, 1080, 720)

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

        # Create the container for input fields
        input_container = QFrame()
        input_container.setStyleSheet("QFrame { border: none; }")  # Remove border from input container
        input_layout = QFormLayout(input_container)
        input_layout.setContentsMargins(0, 0, 0, 0)
        input_layout.setSpacing(10)

        # Create a fixed width for input fields and dropdowns
        fixed_width = 200

        # Number of graphene strips
        num_strips_label = QLabel("Number of Graphene Strips:")
        num_strips_label.setAlignment(Qt.AlignVCenter)  # Ensure the label is vertically centered
        num_strips_input = QLineEdit()
        num_strips_input.setPlaceholderText("Enter Number of Strips")
        num_strips_input.setFixedWidth(fixed_width)
        input_layout.addRow(num_strips_label, num_strips_input)

        # Excitation Voltage per strip
        excitation_voltage_label = QLabel("Excitation Voltage per Strip:")
        excitation_voltage_label.setAlignment(Qt.AlignVCenter)
        excitation_voltage_input = QLineEdit()
        excitation_voltage_input.setPlaceholderText("Enter Excitation Voltage")
        excitation_voltage_input.setFixedWidth(fixed_width)
        input_layout.addRow(excitation_voltage_label, excitation_voltage_input)

        # MUX Frequency
        mux_freq_label = QLabel("MUX Frequency:")
        mux_freq_label.setAlignment(Qt.AlignVCenter)  # Ensure the label is vertically centered
        mux_freq_input = QLineEdit()
        mux_freq_input.setPlaceholderText("Enter MUX Frequency")
        mux_freq_input.setFixedWidth(fixed_width)
        input_layout.addRow(mux_freq_label, mux_freq_input)

        # Wave Frequency
        wave_freq_label = QLabel("Wave Frequency:")
        wave_freq_label.setAlignment(Qt.AlignVCenter)
        wave_freq_input = QLineEdit()
        wave_freq_input.setPlaceholderText("Enter Wave Frequency")
        wave_freq_input.setFixedWidth(fixed_width)
        input_layout.addRow(wave_freq_label, wave_freq_input)

        # Wave Type
        wave_type_label = QLabel("Wave Type:")
        wave_type_label.setAlignment(Qt.AlignVCenter)  # Ensure the label is vertically centered
        wave_type_dropdown = QComboBox()
        wave_type_dropdown.addItem("Block wave")  # Add entry to dropdown
        wave_type_dropdown.addItem("Sine wave")  # Add entry to dropdown
        wave_type_dropdown.addItem("Triangle wave")  # Add entry to dropdown
        wave_type_dropdown.addItem("Sawtooth wave")  # Add entry to dropdown
        wave_type_dropdown.setFixedWidth(fixed_width)
        input_layout.addRow(wave_type_label, wave_type_dropdown)

        # Wave Amplitude
        wave_amp_label = QLabel("Wave Amplitude:")
        wave_amp_label.setAlignment(Qt.AlignVCenter)
        wave_amp_input = QLineEdit()
        wave_amp_input.setPlaceholderText("Enter Wave Amplitude")
        wave_amp_input.setFixedWidth(fixed_width)
        input_layout.addRow(wave_amp_label, wave_amp_input)

        # Duty Cycle
        duty_cycle_label = QLabel("Duty Cycle:")
        duty_cycle_label.setAlignment(Qt.AlignVCenter)
        duty_cycle_input = QLineEdit()
        duty_cycle_input.setPlaceholderText("Enter Duty Cycle")
        duty_cycle_input.setFixedWidth(fixed_width)
        input_layout.addRow(duty_cycle_label, duty_cycle_input)

        # Create a container for buttons with a vertical layout to stack them
        button_container = QFrame()
        button_layout = QVBoxLayout(button_container)
        button_layout.setAlignment(Qt.AlignCenter)  # Center-align the buttons

        # Button to set the parameters and start measurement
        start_button = QPushButton("Start Measurement")
        start_button.setObjectName("startButton")  # Set an object name for styling
        start_button.setFixedSize(fixed_width, 40)  # Set button size
        button_layout.addWidget(start_button)

        # Button to download data as CSV file
        download_button = QPushButton("Download Data")
        download_button.setObjectName("downloadButton")  # Set an object name for styling
        download_button.setFixedSize(fixed_width, 40)  # Set button size
        button_layout.addWidget(download_button)

        input_layout.addRow(button_container)  # Add the button container to the form layout

        # Main layout for top left frame
        top_left_frame_layout = QVBoxLayout(top_left_frame)
        top_left_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        top_left_frame_layout.setSpacing(20)  # Set spacing between elements
        top_left_frame_layout.addWidget(logo_label)
        top_left_frame_layout.addWidget(input_container)

        return top_left_frame

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

        bottom_label = QLabel("Serial Monitor")
        bottom_label.setStyleSheet("QLabel { border: none; }")  # Remove inner border from bottom label
        bottom_label.setAlignment(Qt.AlignCenter)

        bottom_frame_layout = QVBoxLayout(bottom_frame)
        bottom_frame_layout.setContentsMargins(10, 10, 10, 10)  # Set margins
        bottom_frame_layout.setSpacing(20)  # Set spacing between elements
        bottom_frame_layout.addWidget(bottom_label)

        return bottom_frame

# Create the application
app = QApplication([])

# Create and show the main window
window = MainWindow()
window.show()

# Run the application's event loop
app.exec()
