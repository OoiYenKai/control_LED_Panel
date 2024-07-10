#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

// Function to open the serial port
int openSerialPort(const char* port) {
    int serialPort = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
        std::cerr << "Error opening serial port" << std::endl;
    }
    return serialPort;
}

// Function to configure the serial port
void configureSerialPort(int serialPort) {
    struct termios options;
    tcgetattr(serialPort, &options);

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;     // 8 data bits
    options.c_cflag &= ~CRTSCTS; // No hardware flow control
    options.c_cflag |= CREAD | CLOCAL; // Enable receiver, local mode

    tcsetattr(serialPort, TCSANOW, &options);
}

// Function to get user input
void getUserInput(char &char1, char &char2) {
    std::cout << "Enter the first character: ";
    std::cin >> char1;
    std::cout << "Enter the second character: ";
    std::cin >> char2;
}

// Function to construct the command string
void constructCommand(unsigned char command[], char char1, char char2) {
    command[0] = 0x02; // STX
    command[1] = 0xF0; // LED communication code
    command[2] = 0x31;
    command[3] = 0x30;
    command[4] = 0x31;
    command[5] = static_cast<unsigned char>(char1); // User input character 1
    command[6] = static_cast<unsigned char>(char2); // User input character 2
    command[7] = 0x03; // ETX
}

// Function to print the command in hexadecimal format
void printCommand(const unsigned char command[], size_t size) {
    std::cout << "Command: ";
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X ", command[i]);
    }
    std::cout << "\n";
}

// Function to write the command to the serial port
bool writeCommand(int serialPort, const unsigned char command[], size_t size) {
    int bytesWritten = write(serialPort, command, size);
    if (bytesWritten < 0) {
        std::cerr << "Error writing to serial port" << std::endl;
        return false;
    }
    std::cout << "Command sent to LED panel\n";
    return true;
}

// Function to read the response from the serial port
bool readResponse(int serialPort, unsigned char response[], size_t size) {
    usleep(100000); // Sleep for 100 ms

    int bytesRead = read(serialPort, response, size);
    if (bytesRead < 0) {
        std::cerr << "Error reading from serial port" << std::endl;
        return false;
    }

    std::cout << "Response from LED: ";
    for (int i = 0; i < bytesRead; i++) {
        std::cout << "0x" << std::hex << std::uppercase << (int)response[i] << " ";
    }
    std::cout << std::dec << "\n"; // Switch back to decimal output
    return true;
}

// Main function
int main() {
    const char* port = "/dev/ttyUSB0";
    int serialPort = openSerialPort(port);
    if (serialPort == -1) return 1;

    configureSerialPort(serialPort);

    while (true) {
        char char1, char2;
        getUserInput(char1, char2);

        unsigned char command[8];
        constructCommand(command, char1, char2);
        printCommand(command, sizeof(command));

        if (!writeCommand(serialPort, command, sizeof(command))) {
            close(serialPort);
            return 1;
        }

        unsigned char response[8];
        if (!readResponse(serialPort, response, sizeof(response))) {
            close(serialPort);
            return 1;
        }

        std::cout << "Do you want to enter another pair of characters?\nPress any key to continue, n to exit: ";
        char cont;
        std::cin >> cont;
        if (cont == 'n' || cont == 'N') {
            break;
        }      
    }

    close(serialPort);
    return 0;
}