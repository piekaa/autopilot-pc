#ifndef MSFS_CONTROLLER_SERIALCONNECTION_H
#define MSFS_CONTROLLER_SERIALCONNECTION_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <optional>


class SerialConnection {
    HANDLE hSerial;
    bool connected;

public:

    SerialConnection(const std::string& portName) {
        this->connected = false;

        // Open the serial port
        std::wstring widePortName = std::wstring(portName.begin(), portName.end());
        this->hSerial = CreateFileW(
            widePortName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (this->hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening serial port " << portName << std::endl;
            return;
        }

        // Configure serial port parameters
        DCB dcbSerialParams = {0};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(this->hSerial, &dcbSerialParams)) {
            std::cerr << "Error getting serial port state" << std::endl;
            CloseHandle(this->hSerial);
            return;
        }

        // Set baud rate to 115200
        dcbSerialParams.BaudRate = CBR_115200;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

        if (!SetCommState(this->hSerial, &dcbSerialParams)) {
            std::cerr << "Error setting serial port state" << std::endl;
            CloseHandle(this->hSerial);
            return;
        }

        // Set timeouts
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;

        if (!SetCommTimeouts(this->hSerial, &timeouts)) {
            std::cerr << "Error setting serial port timeouts" << std::endl;
            CloseHandle(this->hSerial);
            return;
        }

        this->connected = true;
        std::cout << "Serial port " << portName << " opened successfully at 115200 baud" << std::endl;
    }

    ~SerialConnection() {
        if (this->connected) {
            CloseHandle(this->hSerial);
            this->connected = false;
        }
    }

    HANDLE getHandle() const {
        return this->hSerial;
    }

    bool isConnected() const {
        return this->connected;
    }

    // Read a line from serial port (until '\n')
    // Returns nullopt if no complete line is available
    std::optional<std::string> readLine() {
        if (!this->connected) {
            return std::nullopt;
        }

        static std::string buffer;
        char tempBuffer[256];
        DWORD bytesRead = 0;

        // Read available data
        BOOL readSuccess = ReadFile(this->hSerial, tempBuffer, sizeof(tempBuffer) - 1, &bytesRead, nullptr);
        if (!readSuccess) {
            DWORD error = GetLastError();
            std::cerr << "ReadFile failed with error: " << error << std::endl;
        } else if (bytesRead > 0) {
            tempBuffer[bytesRead] = '\0';
            buffer.append(tempBuffer, bytesRead);
        }

        // Check if we have a complete line (ending with '\n')
        size_t newlinePos = buffer.find('\n');
        if (newlinePos != std::string::npos) {
            std::string line = buffer.substr(0, newlinePos);
            buffer.erase(0, newlinePos + 1);

            // Remove trailing '\r' if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            return line;
        }

        return std::nullopt;
    }

};

#endif //MSFS_CONTROLLER_SERIALCONNECTION_H
