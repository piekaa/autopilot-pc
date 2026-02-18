#include "AutopilotController.h"
#include <iostream>

AutopilotController::AutopilotController(HANDLE simConnectHandle)
    : hSimConnect(simConnectHandle) {
    // Constructor
}

bool AutopilotController::initialize() {
    HRESULT hr;

    // Map SimConnect events to set absolute values

    // Heading set event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_SET, "HEADING_BUG_SET");
    if (hr != S_OK) {
        std::cerr << "Failed to map HEADING_BUG_SET event" << std::endl;
        return false;
    }

    // Vertical Speed set event (in feet per minute)
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_SET, "AP_VS_VAR_SET_ENGLISH");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_VS_VAR_SET_ENGLISH event" << std::endl;
        return false;
    }

    // Altitude set event (in feet)
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_SET, "AP_ALT_VAR_SET_ENGLISH");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_ALT_VAR_SET_ENGLISH event" << std::endl;
        return false;
    }

    // Airspeed set event (in knots)
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_SET, "AP_SPD_VAR_SET");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_SPD_VAR_SET event" << std::endl;
        return false;
    }

    // Autopilot master toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_MASTER, "AP_MASTER");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_MASTER event" << std::endl;
        return false;
    }

    // Heading hold toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_HDG_HOLD, "AP_HDG_HOLD");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_HDG_HOLD event" << std::endl;
        return false;
    }

    // Nav hold (LNAV) toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_NAV1_HOLD, "AP_NAV1_HOLD");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_NAV1_HOLD event" << std::endl;
        return false;
    }

    // Altitude hold toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_ALT_HOLD, "AP_ALT_HOLD");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_ALT_HOLD event" << std::endl;
        return false;
    }

    // Vertical speed hold toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_VS_HOLD, "AP_VS_HOLD");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_VS_HOLD event" << std::endl;
        return false;
    }

    // Auto-throttle arm toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AUTO_THROTTLE_ARM, "AUTO_THROTTLE_ARM");
    if (hr != S_OK) {
        std::cerr << "Failed to map AUTO_THROTTLE_ARM event" << std::endl;
        return false;
    }

    // Flight level change (VNAV) toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_FLC, "FLIGHT_LEVEL_CHANGE");
    if (hr != S_OK) {
        std::cerr << "Failed to map FLIGHT_LEVEL_CHANGE event" << std::endl;
        return false;
    }

    std::cout << "AutopilotController initialized successfully." << std::endl;
    return true;
}

void AutopilotController::setHeading(int value) {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_HEADING_SET,
        value, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Set Heading = " << value << std::endl;
}

void AutopilotController::setVerticalSpeed(int value) {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_VS_SET,
        value, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Set Vertical Speed = " << value << " fpm" << std::endl;
}

void AutopilotController::setAltitude(int value) {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_SET,
        value, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Set Altitude = " << value << " ft" << std::endl;
}

void AutopilotController::setSpeed(int value) {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_SPEED_SET,
        value, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Set Speed = " << value << " kts" << std::endl;
}

void AutopilotController::toggleAutopilot() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_MASTER,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Autopilot Master" << std::endl;
}

void AutopilotController::toggleHeadingHold() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_HDG_HOLD,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Heading Hold" << std::endl;
}

void AutopilotController::toggleNavHold() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_NAV1_HOLD,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Nav Hold (LNAV)" << std::endl;
}

void AutopilotController::toggleAltitudeHold() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_ALT_HOLD,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Altitude Hold" << std::endl;
}

void AutopilotController::toggleVerticalSpeedHold() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_VS_HOLD,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Vertical Speed Hold" << std::endl;
}

void AutopilotController::toggleAutoThrottle() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AUTO_THROTTLE_ARM,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Auto-Throttle" << std::endl;
}

void AutopilotController::toggleFlightLevelChange() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AP_FLC,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Toggle Flight Level Change (VNAV)" << std::endl;
}

void AutopilotController::processCommand(const std::string& commandType, int value) {
    // Process command based on type
    if (commandType == "H") {
        setHeading(value);
    } else if (commandType == "VS") {
        setVerticalSpeed(value);
    } else if (commandType == "A") {
        setAltitude(value);
    } else if (commandType == "S") {
        setSpeed(value);
    } else {
        // Ignore unknown commands
        std::cout << "Ignoring unknown command type: " << commandType << std::endl;
    }
}

void AutopilotController::processCommand(const std::string& commandType) {
    // Process command without value
    if (commandType == "AP") {
        toggleAutopilot();
    } else if (commandType == "AP_HEADING") {
        toggleHeadingHold();
    } else if (commandType == "AP_LNAV") {
        toggleNavHold();
    } else if (commandType == "AP_VNAV") {
        toggleFlightLevelChange();
    } else if (commandType == "AP_SPEED") {
        toggleAutoThrottle();
    } else if (commandType == "AP_ALTITUDE") {
        toggleAltitudeHold();
    } else if (commandType == "AP_VS") {
        toggleVerticalSpeedHold();
    } else {
        std::cout << "Ignoring unknown command type: " << commandType << std::endl;
    }
}
