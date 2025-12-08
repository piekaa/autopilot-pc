#include "CommandProcessor.h"
#include <iostream>

CommandProcessor::CommandProcessor(SerialPortReader& serial, AutopilotController& ap)
    : serialPort(serial), autopilot(ap), waitingForValue(false) {
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

        // State machine: first line is command type, second line is value
        if (!waitingForValue) {
            // This is the command type line
            currentCommandType = line;
            waitingForValue = true;
            std::cout << "Received command type: " << currentCommandType << std::endl;
        } else {
            // This is the command value line
            std::cout << "Received command value: " << line << std::endl;

            // Process the complete command
            autopilot.processCommand(currentCommandType, line);

            // Reset state machine for next command
            reset();
        }
    }
}

void CommandProcessor::reset() {
    currentCommandType.clear();
    waitingForValue = false;
}
