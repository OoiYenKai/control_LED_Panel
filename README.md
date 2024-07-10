# LED Panel Serial Communication
This project allows you to send commands to an LED panel via an RS-232 serial port. The commands are constructed based on user input and sent to the LED panel to display specific characters.

# Prerequisites
- C++ Compiler (e.g., g++)
- POSIX-compliant operating system (e.g., Linux)
- Access to a serial port (e.g., /dev/ttyUSB0)
- LED panel connected to the serial port with a straight through RS232 cable

# Getting Started
### Cloning the Repository
First, clone the repository to your local machine:

```
https://github.com/OoiYenKai/control_LED_Panel.git
cd /path/to/your/repository 
```

### Compiling the Code
Compile the code using a C++ compiler:
```
g++ -o control_LED control_LED.cpp
```
### Running the Program
Run the compiled program:
```
./control_LED
```
# Code Overview
### Functions
1. openSerialPort(const char port):
    - Opens the specified serial port.
    - Returns the file descriptor for the serial port or -1 if there's an error.
    - configureSerialPort(int serialPort):

2. Configures the serial port with the following settings:
    - Baud rate: 9600
    - No parity
    - 1 stop bit
    - 8 data bits
    - No hardware flow control

3. getUserInput(char &char1, char &char2):
    - Prompts the user to enter two characters.

4. constructCommand(unsigned char command[], char char1, char char2):
    - Constructs the command string to be sent to the LED panel using the user-provided characters.

5. printCommand(const unsigned char command[], size_t size):
    - Prints the command in hexadecimal format.

6. writeCommand(int serialPort, const unsigned char command[], size_t size):
    - Writes the command to the serial port.
    - Returns true if the command is successfully written, false otherwise.

7. readResponse(int serialPort, unsigned char response[], size_t size):
    - Reads the response from the serial port.
    - Returns true if the response is successfully read, false otherwise.

### Main Function
The main function performs the following steps:

1. Opens and configures the serial port.
2. Continuously prompts the user to enter two characters.
3. Constructs the command based on user input.
4. Sends the command to the LED panel and reads the response.
5. Asks the user if they want to enter another pair of characters or exit.

# Usage
1. Ensure your LED panel is connected to the specified serial port (default: /dev/ttyUSB0).
2. Run the program and follow the prompts to enter characters to be displayed on the LED panel.
3. The program will display the command sent to the LED panel and the response received from the LED panel.