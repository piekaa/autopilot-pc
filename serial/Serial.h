#ifndef MSFS_CONTROLLER_SERIAL_H
#define MSFS_CONTROLLER_SERIAL_H

#include <Windows.h>
#include <string>

#include "CommandProcessor.h"
#include "SerialConnection.h"

class Serial {
    SerialConnection* connection;
    AutopilotWriter* autopilotWriter = nullptr;

public:
    Serial(const std::string& portName = "COM5") {
        this->connection = new SerialConnection(portName);
    }

    void setAutopilotWriter(AutopilotWriter* autopilotWriter) {
        this->autopilotWriter = autopilotWriter;
    }

    ~Serial() {
        delete this->connection;
    }

    bool isConnected() const {
        return this->connection->isConnected();
    }

    void handleCommand() {
        if (auto line = connection->readLine()) {
            if (auto cmd = CommandProcessor::toValidCommand(line.value())) {
                auto [command, value] = *cmd;
                if (command == "H") {
                    autopilotWriter->setHeading(std::stoi(value));
                }
                if (command == "S") {
                    autopilotWriter->setSpeed(std::stoi(value));
                }
                if (command == "A") {
                    autopilotWriter->setAltitude(std::stoi(value));
                }
                if (command == "VS") {
                    autopilotWriter->setVerticalSpeed(std::stoi(value));
                }
                if (command == "AP") {
                    autopilotWriter->toggleAutopilot();
                }
                if (command == "AP_HEADING") {
                    autopilotWriter->toggleHeading();
                }
                if (command == "AP_VS") {
                    autopilotWriter->toggleVerticalSpeed();
                }
                if (command == "AP_VNAV") {
                    autopilotWriter->toggleVNav();
                }
                if (command == "AP_LNAV") {
                    autopilotWriter->toggleLNav();
                }
                if (command == "AP_SPEED") {
                    autopilotWriter->toggleSpeed();
                }
                if (command == "AP_ALTITUDE") {
                    autopilotWriter->toggleAltitude();
                }
            }
        }
    }

    bool write(const std::string& data) {

        std::cout << data << std::endl;

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
