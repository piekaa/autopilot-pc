#include "AutopilotController.h"
#include <iostream>

AutopilotController::AutopilotController(HANDLE simConnectHandle)
    : hSimConnect(simConnectHandle) {
    // Constructor
}

bool AutopilotController::initialize() {
    HRESULT hr;

    // Map SimConnect events to event IDs (like registering callbacks in Java)

    // Heading events
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_INC, "HEADING_BUG_INC");
    if (hr != S_OK) {
        std::cerr << "Failed to map HEADING_BUG_INC event" << std::endl;
        return false;
    }

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_DEC, "HEADING_BUG_DEC");
    if (hr != S_OK) {
        std::cerr << "Failed to map HEADING_BUG_DEC event" << std::endl;
        return false;
    }

    // Vertical Speed events
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_INC, "AP_VS_VAR_INC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_VS_VAR_INC event" << std::endl;
        return false;
    }

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_DEC, "AP_VS_VAR_DEC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_VS_VAR_DEC event" << std::endl;
        return false;
    }

    // Altitude events
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_INC, "AP_ALT_VAR_INC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_ALT_VAR_INC event" << std::endl;
        return false;
    }

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_DEC, "AP_ALT_VAR_DEC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_ALT_VAR_DEC event" << std::endl;
        return false;
    }

    // Airspeed events
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_INC, "AP_SPD_VAR_INC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_SPD_VAR_INC event" << std::endl;
        return false;
    }

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_DEC, "AP_SPD_VAR_DEC");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_SPD_VAR_DEC event" << std::endl;
        return false;
    }

    std::cout << "AutopilotController initialized successfully." << std::endl;
    return true;
}

void AutopilotController::increaseHeading() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_HEADING_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Heading +" << std::endl;
}

void AutopilotController::decreaseHeading() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_HEADING_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Heading -" << std::endl;
}

void AutopilotController::increaseVerticalSpeed() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_VS_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Vertical Speed +" << std::endl;
}

void AutopilotController::decreaseVerticalSpeed() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_VS_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Vertical Speed -" << std::endl;
}

void AutopilotController::increaseAltitude() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Altitude +" << std::endl;
}

void AutopilotController::decreaseAltitude() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Altitude -" << std::endl;
}

void AutopilotController::increaseSpeed() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_SPEED_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Speed +" << std::endl;
}

void AutopilotController::decreaseSpeed() {
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_SPEED_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Speed -" << std::endl;
}

void AutopilotController::processCommand(const std::string& commandType, const std::string& commandValue) {
    // Process command based on type and value (like a switch statement in Java)
    if (commandType == "H") {
        if (commandValue == "+") {
            increaseHeading();
        } else if (commandValue == "-") {
            decreaseHeading();
        } else {
            std::cerr << "Unknown heading command value: " << commandValue << std::endl;
        }
    } else if (commandType == "VS") {
        if (commandValue == "+") {
            increaseVerticalSpeed();
        } else if (commandValue == "-") {
            decreaseVerticalSpeed();
        } else {
            std::cerr << "Unknown VS command value: " << commandValue << std::endl;
        }
    } else if (commandType == "A") {
        if (commandValue == "+") {
            increaseAltitude();
        } else if (commandValue == "-") {
            decreaseAltitude();
        } else {
            std::cerr << "Unknown altitude command value: " << commandValue << std::endl;
        }
    } else if (commandType == "S") {
        if (commandValue == "+") {
            increaseSpeed();
        } else if (commandValue == "-") {
            decreaseSpeed();
        } else {
            std::cerr << "Unknown speed command value: " << commandValue << std::endl;
        }
    } else {
        // Ignore unknown commands as requested
        std::cout << "Ignoring unknown command type: " << commandType << std::endl;
    }
}
