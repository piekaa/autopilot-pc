#include "CommandProcessor.h"
#include <iostream>
#include <sstream>

CommandProcessor::CommandProcessor(SerialPortReader& serial, AutopilotController& ap)
    : serialPort(serial), autopilot(ap) {
    // Constructor
}

void CommandProcessor::processIncomingData() {
    if (!serialPort.isConnected()) {
        return;
    }

    // Check if data is available
    if (!serialPort.isDataAvailable()) {
        return;
    }

    // Read a line from serial port
    std::string line;
    if (serialPort.readLine(line)) {
        if (line.empty()) {
            return;
        }

        parseCommand(line);
    }
}

void CommandProcessor::parseCommand(const std::string& line) {
    // Parse single-line command: "CMD value" or "CMD SUBCOMMAND"
    std::istringstream iss(line);
    std::string command;

    iss >> command;

    // Ignore debug logs that start with X
    if (command == "X") {
        std::cout << "Debug log: " << line << std::endl;
        return;
    }

    // Check for AP TOGGLE command
    if (command == "AP") {
        std::string subcommand;
        iss >> subcommand;

        if (subcommand == "TOGGLE") {
            std::cout << "Received command: AP TOGGLE" << std::endl;
            autopilot.processCommand(command);
            return;
        } else {
            std::cerr << "Invalid AP subcommand: " << subcommand << std::endl;
            return;
        }
    }

    // Read the value for other commands
    int value;
    if (!(iss >> value)) {
        std::cerr << "Invalid command format: " << line << std::endl;
        return;
    }

    std::cout << "Received command: " << command << " " << value << std::endl;

    // Process the command
    autopilot.processCommand(command, value);
}
