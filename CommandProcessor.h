#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include "SerialPortReader.h"
#include "AutopilotController.h"

/**
 * CommandProcessor class (Java-style)
 * Reads commands from serial port and processes them
 * Format: Single line commands, first word is command name
 *         H 100 - set heading to 100
 *         S 231 - set speed to 231
 *         A 13000 - set altitude to 13000
 *         VS -1000 - set vertical speed to -1000
 *         X ... - debug log (ignored)
 */
class CommandProcessor {
private:
    SerialPortReader& serialPort;
    AutopilotController& autopilot;

public:
    // Constructor
    CommandProcessor(SerialPortReader& serial, AutopilotController& ap);

    // Process incoming serial data (call this repeatedly in a loop)
    void processIncomingData();

    // Parse and execute a command line
    void parseCommand(const std::string& line);
};

#endif // COMMANDPROCESSOR_H
