#ifndef MSFS_CONTROLLER_SERIAL_H
#define MSFS_CONTROLLER_SERIAL_H

#include <Windows.h>
#include <string>

#include "CommandProcessor.h"
#include "../tcp/TcpServer.h"

class Serial {
    TcpServer* connection;
    AutopilotWriter* autopilotWriter = nullptr;

public:
    Serial(int port = 5000) {
        this->connection = new TcpServer(port);
    }

    void setAutopilotWriter(AutopilotWriter* autopilotWriter) {
        this->autopilotWriter = autopilotWriter;
    }

    ~Serial() {
        delete this->connection;
    }

    bool isConnected() {
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
        return this->connection->write(data);
    }
};

#endif //MSFS_CONTROLLER_SERIAL_H
