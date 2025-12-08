#ifndef AUTOPILOTCONTROLLER_H
#define AUTOPILOTCONTROLLER_H

#include <Windows.h>
#include <SimConnect.h>
#include <string>

/**
 * AutopilotController class (Java-style)
 * Handles sending autopilot commands to MSFS
 */
class AutopilotController {
private:
    HANDLE hSimConnect;

    // Event IDs for SimConnect events
    enum EVENT_ID {
        EVENT_HEADING_INC,
        EVENT_HEADING_DEC,
        EVENT_VS_INC,
        EVENT_VS_DEC,
        EVENT_ALT_INC,
        EVENT_ALT_DEC,
        EVENT_SPEED_INC,
        EVENT_SPEED_DEC
    };

public:
    // Constructor
    AutopilotController(HANDLE simConnectHandle);

    // Initialize - map events
    bool initialize();

    // Command methods (like Java methods)
    void increaseHeading();
    void decreaseHeading();
    void increaseVerticalSpeed();
    void decreaseVerticalSpeed();
    void increaseAltitude();
    void decreaseAltitude();
    void increaseSpeed();
    void decreaseSpeed();

    // Process a command string
    void processCommand(const std::string& commandType, const std::string& commandValue);
};

#endif // AUTOPILOTCONTROLLER_H
