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
        EVENT_HEADING_SET,
        EVENT_VS_SET,
        EVENT_ALT_SET,
        EVENT_SPEED_SET,
        EVENT_AP_MASTER,
        EVENT_AP_HDG_HOLD,
        EVENT_AP_NAV1_HOLD,
        EVENT_AP_ALT_HOLD,
        EVENT_AP_VS_HOLD,
        EVENT_AUTO_THROTTLE_ARM,
        AP_SPEED_SLOT_INDEX_SET,
        AP_PANEL_SPEED_ON,
        EVENT_AP_FLC
    };

    // Request IDs for SimConnect requests
    enum REQUEST_ID {
        REQUEST_ENUMERATE_INPUT_EVENTS = 1000
    };

    // InputEvent hash storage
    UINT64 fccSpeedHash;
    bool fccSpeedHashFound;

public:
    // Constructor
    AutopilotController(HANDLE simConnectHandle);

    // Initialize - map events
    bool initialize();

    // Command methods to set absolute values
    void setHeading(int value);
    void setVerticalSpeed(int value);
    void setAltitude(int value);
    void setSpeed(int value);

    // Toggle autopilot master
    void toggleAutopilot();

    // Toggle autopilot modes
    void toggleHeadingHold();
    void toggleNavHold();
    void toggleAltitudeHold();
    void toggleVerticalSpeedHold();
    void toggleAutoThrottle();
    void toggleFlightLevelChange();

    // Enhanced speed control for 737 compatibility
    void enableSpeedModeAndSet(int speedKnots);

    // InputEvent API methods for 737 FCC_SPEED control
    void processInputEventEnumeration(SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS* pData);
    void triggerFccSpeed();

    // Process a command with value
    void processCommand(const std::string& commandType, int value);

    // Process a command without value
    void processCommand(const std::string& commandType);
};

#endif // AUTOPILOTCONTROLLER_H
