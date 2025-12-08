#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include "SerialPortReader.h"
#include "AutopilotController.h"

/**
 * CommandProcessor class (Java-style)
 * Reads commands from serial port and processes them
 * Format: Line 1 = command type (H, VS, A, S)
 *         Line 2 = command value (+, -, or other)
 */
class CommandProcessor {
private:
    SerialPortReader& serialPort;
    AutopilotController& autopilot;
    std::string currentCommandType;
    bool waitingForValue;

public:
    // Constructor
    CommandProcessor(SerialPortReader& serial, AutopilotController& ap);

    // Process incoming serial data (call this repeatedly in a loop)
    void processIncomingData();

    // Reset the state machine
    void reset();
};

#endif // COMMANDPROCESSOR_H
