#include "AutopilotController.h"
#include <iostream>
#include <chrono>
#include <thread>

AutopilotController::AutopilotController(HANDLE simConnectHandle)
    : hSimConnect(simConnectHandle), fccSpeedHash(0), fccSpeedHashFound(false) {
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

    // Speed hold events (for 737 compatibility)
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, AP_PANEL_SPEED_ON, "AP_PANEL_SPEED_ON");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_PANEL_SPEED_ON event" << std::endl;
        return false;
    }

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, AP_SPEED_SLOT_INDEX_SET, "AP_AIRSPEED_HOLD");
    if (hr != S_OK) {
        std::cerr << "Failed to map AP_AIRSPEED_HOLD event" << std::endl;
        return false;
    }

    // Flight level change (VNAV) toggle event
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AP_FLC, "FLIGHT_LEVEL_CHANGE");
    if (hr != S_OK) {
        std::cerr << "Failed to map FLIGHT_LEVEL_CHANGE event" << std::endl;
        return false;
    }

    std::cout << "AutopilotController initialized successfully." << std::endl;

    // Enumerate InputEvents to find FCC_SPEED for 737 compatibility
    std::cout << "Enumerating InputEvents for aircraft-specific controls..." << std::endl;
    hr = SimConnect_EnumerateInputEvents(hSimConnect, REQUEST_ENUMERATE_INPUT_EVENTS);
    if (hr != S_OK) {
        std::cerr << "Warning: Failed to enumerate InputEvents (this is normal if aircraft doesn't support it)" << std::endl;
    }

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
    SimConnect_TransmitClientEvent_EX1(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_SET,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY, value);

    SimConnect_TransmitClientEvent_EX1(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_SET,
                                       SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY, value, 1);

    SimConnect_TransmitClientEvent_EX1(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_SET,
                                       SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY, value, 2);

    SimConnect_TransmitClientEvent_EX1(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_ALT_SET,
                                       SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY, value, 3);
    std::cout << "Command: Set Altitude = " << value << " ft" << std::endl;
}

void AutopilotController::setSpeed(int value) {
    // AP_SPD_VAR_SET requires two parameters: speed in knots and slot index (0-4)
    // Slot index: 0 = all slots, 1-4 = specific managed speed slots
    // Using slot 0 to set all managed speed references
    DWORD combinedValue = MAKELONG(value, 0); // speed in low word, slot index (0) in high word

    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_SPEED_SET,
        combinedValue, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Set Speed = " << value << " kts (slot 0)" << std::endl;
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
    // For 737: Sequence to enable autothrottle speed mode
    // 1. Arm autothrottle
    SimConnect_TransmitClientEvent(hSimConnect, SIMCONNECT_OBJECT_ID_USER, EVENT_AUTO_THROTTLE_ARM,
        0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
    std::cout << "Command: Arm Auto-Throttle" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    triggerFccSpeed();
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

void AutopilotController::processInputEventEnumeration(SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS* pData) {
    // Process enumerated InputEvents to find FCC_SPEED
    std::cout << "Processing InputEvent enumeration batch (RequestID: " << pData->dwRequestID
              << ", Entry count: " << pData->dwEntryNumber << "/" << pData->dwOutOf << ")" << std::endl;

    // Iterate through all InputEvent descriptors in this batch
    for (DWORD i = 0; i < pData->dwArraySize; i++) {
        SIMCONNECT_INPUT_EVENT_DESCRIPTOR* descriptor = &pData->rgData[i];
        std::string eventName(descriptor->Name);

        std::cout << "  Found InputEvent: " << eventName << " (hash: " << descriptor->Hash << ")" << std::endl;

        // Check if this is FCC_SPEED
        if (eventName == "FCC_SPEED") {
            fccSpeedHash = descriptor->Hash;
            fccSpeedHashFound = true;
            std::cout << "*** Found FCC_SPEED InputEvent! Hash: " << fccSpeedHash << " ***" << std::endl;
        }
    }

    // Log completion when we've received all batches
    if (pData->dwEntryNumber == pData->dwOutOf - 1) {
        std::cout << "InputEvent enumeration complete." << std::endl;
        if (fccSpeedHashFound) {
            std::cout << "FCC_SPEED is available for this aircraft." << std::endl;
        } else {
            std::cout << "FCC_SPEED not found - this aircraft may not support it." << std::endl;
        }
    }
}

void AutopilotController::triggerFccSpeed() {
    if (!fccSpeedHashFound) {
        std::cerr << "Error: FCC_SPEED InputEvent not available for this aircraft." << std::endl;
        std::cerr << "       Make sure you're in a 737 and SimConnect has enumerated InputEvents." << std::endl;
        return;
    }

    std::cout << "Triggering FCC_SPEED InputEvent (hash: " << fccSpeedHash << ")" << std::endl;

    // Set FCC_SPEED to 1.0 to enable speed mode
    // Based on 737 behavior, setting FCC_SPEED to 1.0 activates the speed mode
    double value = 1.0;
    HRESULT hr = SimConnect_SetInputEvent(hSimConnect, fccSpeedHash, sizeof(value), &value);

    if (hr == S_OK) {
        std::cout << "Successfully triggered FCC_SPEED InputEvent." << std::endl;
    } else {
        std::cerr << "Failed to trigger FCC_SPEED InputEvent (HRESULT: " << hr << ")" << std::endl;
    }
}
