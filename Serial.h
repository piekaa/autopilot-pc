#ifndef MSFS_CONTROLLER_SERIAL_H
#define MSFS_CONTROLLER_SERIAL_H

#include <Windows.h>
#include <string>
#include "SerialConnection.h"

class Serial {
    SerialConnection* connection;

public:
    Serial(const std::string& portName = "COM5") {
        this->connection = new SerialConnection(portName);
    }

    ~Serial() {
        delete this->connection;
    }

    bool isConnected() const {
        return this->connection->isConnected();
    }

    bool write(const std::string& data) {
        if (!this->connection->isConnected()) {
            return false;
        }

        DWORD bytesWritten;
        bool result = WriteFile(
            this->connection->getHandle(),
            data.c_str(),
            data.length(),
            &bytesWritten,
            nullptr
        );

        return result && (bytesWritten == data.length());
    }
};

#endif //MSFS_CONTROLLER_SERIAL_H
