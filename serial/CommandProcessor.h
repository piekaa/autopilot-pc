#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <optional>
#include "../SerialPortReader.h"
#include "../AutopilotController.h"

/**
 * CommandProcessor class (Java-style)
 * Reads commands from serial port and processes them
 * Format: Single line commands, first word is command name
 *         H 100 - set heading to 100
 *         S 231 - set speed to 231
 *         A 13000 - set altitude to 13000
 *         VS -1000 - set vertical speed to -1000
 *         AP TOGGLE - toggle autopilot master
 *         AP_HEADING TOGGLE - toggle heading hold mode
 *         AP_VNAV TOGGLE - toggle VNAV mode
 *         AP_LNAV TOGGLE - toggle LNAV mode
 *         AP_SPEED TOGGLE - toggle speed/auto-throttle control
 *         AP_ALTITUDE TOGGLE - toggle altitude hold mode
 *         AP_VS TOGGLE - toggle vertical speed mode
 *         X ... - debug log (ignored)
 */
class CommandProcessor {
    // Parse and execute a command line
    // Returns optional pair of command type and command value (nullopt if invalid)
    static std::optional<std::pair<std::string, std::string>> parseCommand(const std::string &line) {
        // Parse single-line command: "CMD value" or "CMD SUBCOMMAND"
        std::istringstream iss(line);
        std::string command;

        iss >> command;

        // Ignore debug logs that start with X
        if (command == "X") {
            std::cout << "Debug log: " << line << std::endl;
            return std::nullopt;
        }

        // Check for toggle commands (AP, AP_HEADING, AP_VNAV, AP_LNAV, AP_SPEED, AP_ALTITUDE, AP_VS)
        if (command == "AP" || command == "AP_HEADING" || command == "AP_VNAV" ||
            command == "AP_LNAV" || command == "AP_SPEED" || command == "AP_ALTITUDE" ||
            command == "AP_VS") {
            std::string subcommand;
            iss >> subcommand;

            if (subcommand == "TOGGLE") {
                std::cout << "Received command: " << command << " TOGGLE" << std::endl;
                return std::make_pair(command, "TOGGLE");
            } else {
                std::cerr << "Invalid " << command << " subcommand: " << subcommand << std::endl;
                return std::nullopt;
            }
        }

        // Read the value for other commands
        int value;
        if (!(iss >> value)) {
            std::cerr << "Invalid command format: " << line << std::endl;
            return std::nullopt;
        }

        std::cout << "Received command: " << command << " " << value << std::endl;

        return std::make_pair(command, std::to_string(value));
    }

public:
    // Process incoming serial data (call this repeatedly in a loop)
    // Returns optional pair of command type and command value (nullopt if invalid)
    static std::optional<std::pair<std::string, std::string>> toValidCommand(std::string line) {
        return parseCommand(line);
    }
};

#endif // COMMANDPROCESSOR_H
