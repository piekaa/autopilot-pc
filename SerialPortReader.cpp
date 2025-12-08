#include "SerialPortReader.h"
#include <iostream>

SerialPortReader::SerialPortReader(const std::string& port)
    : hSerial(INVALID_HANDLE_VALUE), portName(port), connected(false) {
    // Constructor
}

SerialPortReader::~SerialPortReader() {
    close();
}

bool SerialPortReader::open(int baudRate) {
    if (connected) {
        std::cout << "Serial port already open!" << std::endl;
        return true;
    }

    // Open the serial port
    hSerial = CreateFileA(
        portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open serial port " << portName << std::endl;
        DWORD error = GetLastError();
        std::cerr << "Error code: " << error << std::endl;
        return false;
    }

    // Configure serial port parameters
    dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to get serial port state" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to set serial port state" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    // Set timeouts
    timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Failed to set serial port timeouts" << std::endl;
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    connected = true;
    std::cout << "Successfully opened serial port " << portName << " at " << baudRate << " baud" << std::endl;
    return true;
}

void SerialPortReader::close() {
    if (connected && hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        connected = false;
        std::cout << "Serial port closed." << std::endl;
    }
}

bool SerialPortReader::isConnected() const {
    return connected;
}

bool SerialPortReader::readLine(std::string& line) {
    if (!connected) {
        return false;
    }

    line.clear();
    char buffer[1];
    DWORD bytesRead;

    // Read character by character until newline
    while (true) {
        if (!ReadFile(hSerial, buffer, 1, &bytesRead, nullptr)) {
            std::cerr << "Error reading from serial port" << std::endl;
            return false;
        }

        if (bytesRead == 1) {
            if (buffer[0] == '\n') {
                // End of line
                return true;
            } else if (buffer[0] == '\r') {
                // Ignore carriage return
                continue;
            } else {
                line += buffer[0];
            }
        } else {
            // No data available, return what we have so far
            if (!line.empty()) {
                return true;
            }
            return false;
        }
    }
}

bool SerialPortReader::isDataAvailable() {
    if (!connected) {
        return false;
    }

    DWORD errors;
    COMSTAT status;

    if (!ClearCommError(hSerial, &errors, &status)) {
        return false;
    }

    return status.cbInQue > 0;
}
