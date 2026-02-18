#include "AutopilotDataReader.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

AutopilotDataReader::AutopilotDataReader(HANDLE simConnectHandle, SerialPortReader& serial)
    : hSimConnect(simConnectHandle), serialPort(serial),
      currentHeading(0.0), currentVerticalSpeed(0.0), currentAltitude(0.0), currentSpeed(0.0), currentApMaster(0.0),
      currentHeadingHold(0.0), currentNavHold(0.0), currentAltitudeHold(0.0), currentVsHold(0.0),
      currentAirspeedHold(0.0), currentFlc(0.0),
      lastSentHeading(-999.0), lastSentVerticalSpeed(-999.0), lastSentAltitude(-999.0), lastSentSpeed(-999.0), lastSentApMaster(-999.0),
      lastSentHeadingHold(-999.0), lastSentNavHold(-999.0), lastSentAltitudeHold(-999.0), lastSentVsHold(-999.0),
      lastSentAirspeedHold(-999.0), lastSentFlc(-999.0),
      dataReceived(false) {
    // Initialize timing
    lastFullUpdate = std::chrono::steady_clock::now();
    lastRequest = std::chrono::steady_clock::now();
}

bool AutopilotDataReader::initialize() {
    HRESULT hr;

    // Define all autopilot data we want to receive
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT HEADING LOCK DIR", "degrees", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add heading definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT VERTICAL HOLD VAR", "feet per minute", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add vertical speed definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT ALTITUDE LOCK VAR", "feet", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add altitude definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT AIRSPEED HOLD VAR", "knots", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add airspeed definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT MASTER", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot master definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT HEADING LOCK", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot heading lock definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT NAV1 LOCK", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot nav1 lock definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT ALTITUDE LOCK", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot altitude lock definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT VERTICAL HOLD", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot vertical hold definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT AIRSPEED HOLD", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot airspeed hold definition" << std::endl;
        return false;
    }

    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_AUTOPILOT_DATA,
        "AUTOPILOT FLIGHT LEVEL CHANGE", "Bool", SIMCONNECT_DATATYPE_FLOAT64);
    if (hr != S_OK) {
        std::cerr << "Failed to add autopilot flight level change definition" << std::endl;
        return false;
    }

    std::cout << "AutopilotDataReader initialized successfully." << std::endl;
    return true;
}

void AutopilotDataReader::requestData() {
    HRESULT hr = SimConnect_RequestDataOnSimObject(
        hSimConnect,
        REQUEST_AUTOPILOT_DATA,
        DEFINITION_AUTOPILOT_DATA,
        SIMCONNECT_OBJECT_ID_USER,
        SIMCONNECT_PERIOD_ONCE
    );

    if (hr != S_OK) {
        std::cerr << "Failed to request autopilot data" << std::endl;
    }
}

void AutopilotDataReader::processMessage(SIMCONNECT_RECV* pData) {
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            if (pObjData->dwRequestID == REQUEST_AUTOPILOT_DATA) {
                auto* apData = (AutopilotData*)&pObjData->dwData;

                currentHeading = apData->heading;
                currentVerticalSpeed = apData->verticalSpeed;
                currentAltitude = apData->altitude;
                currentSpeed = apData->speed;
                currentApMaster = apData->apMaster;
                currentHeadingHold = apData->headingHold;
                currentNavHold = apData->navHold;
                currentAltitudeHold = apData->altitudeHold;
                currentVsHold = apData->vsHold;
                currentAirspeedHold = apData->airspeedHold;
                currentFlc = apData->flc;

                dataReceived = true;

                // Check for changes and send updates
                if (hasChanged(currentHeading, lastSentHeading, 0.5)) {
                    sendToSerial("H", currentHeading);
                    lastSentHeading = currentHeading;
                }

                if (hasChanged(currentVerticalSpeed, lastSentVerticalSpeed, 10.0)) {
                    sendToSerial("VS", currentVerticalSpeed);
                    lastSentVerticalSpeed = currentVerticalSpeed;
                }

                if (hasChanged(currentAltitude, lastSentAltitude, 10.0)) {
                    sendToSerial("A", currentAltitude);
                    lastSentAltitude = currentAltitude;
                }

                if (hasChanged(currentSpeed, lastSentSpeed, 0.5)) {
                    sendToSerial("S", currentSpeed);
                    lastSentSpeed = currentSpeed;
                }

                if (hasChanged(currentApMaster, lastSentApMaster, 0.1)) {
                    sendApStatusToSerial(currentApMaster > 0.5);
                    lastSentApMaster = currentApMaster;
                }

                if (hasChanged(currentHeadingHold, lastSentHeadingHold, 0.1)) {
                    sendModeStatusToSerial("AP_HEADING", currentHeadingHold > 0.5);
                    lastSentHeadingHold = currentHeadingHold;
                }

                if (hasChanged(currentNavHold, lastSentNavHold, 0.1)) {
                    sendModeStatusToSerial("AP_LNAV", currentNavHold > 0.5);
                    lastSentNavHold = currentNavHold;
                }

                if (hasChanged(currentAltitudeHold, lastSentAltitudeHold, 0.1)) {
                    sendModeStatusToSerial("AP_ALTITUDE", currentAltitudeHold > 0.5);
                    lastSentAltitudeHold = currentAltitudeHold;
                }

                if (hasChanged(currentVsHold, lastSentVsHold, 0.1)) {
                    sendModeStatusToSerial("AP_VS", currentVsHold > 0.5);
                    lastSentVsHold = currentVsHold;
                }

                if (hasChanged(currentAirspeedHold, lastSentAirspeedHold, 0.1)) {
                    sendModeStatusToSerial("AP_SPEED", currentAirspeedHold > 0.5);
                    lastSentAirspeedHold = currentAirspeedHold;
                }

                if (hasChanged(currentFlc, lastSentFlc, 0.1)) {
                    sendModeStatusToSerial("AP_VNAV", currentFlc > 0.5);
                    lastSentFlc = currentFlc;
                }
            }
            break;
        }

        case SIMCONNECT_RECV_ID_EXCEPTION: {
            auto* pException = (SIMCONNECT_RECV_EXCEPTION*)pData;
            std::cerr << "SimConnect exception in AutopilotDataReader: " << pException->dwException << std::endl;
            break;
        }

        default:
            break;
    }
}

void AutopilotDataReader::update() {
    auto now = std::chrono::steady_clock::now();

    // Request data every 100ms
    auto timeSinceRequest = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRequest);
    if (timeSinceRequest.count() >= 100) {
        requestData();
        lastRequest = now;
    }

    // Send all values every 5 seconds
    auto timeSinceFullUpdate = std::chrono::duration_cast<std::chrono::seconds>(now - lastFullUpdate);
    if (timeSinceFullUpdate.count() >= 5) {
        sendAllValues();
        lastFullUpdate = now;
    }
}

void AutopilotDataReader::sendAllValues() {
    if (!dataReceived) {
        return;
    }

    std::cout << "Sending periodic update of all values..." << std::endl;

    sendToSerial("H", currentHeading);
    lastSentHeading = currentHeading;

    sendToSerial("VS", currentVerticalSpeed);
    lastSentVerticalSpeed = currentVerticalSpeed;

    sendToSerial("A", currentAltitude);
    lastSentAltitude = currentAltitude;

    sendToSerial("S", currentSpeed);
    lastSentSpeed = currentSpeed;

    sendApStatusToSerial(currentApMaster > 0.5);
    lastSentApMaster = currentApMaster;

    sendModeStatusToSerial("AP_HEADING", currentHeadingHold > 0.5);
    lastSentHeadingHold = currentHeadingHold;

    sendModeStatusToSerial("AP_LNAV", currentNavHold > 0.5);
    lastSentNavHold = currentNavHold;

    sendModeStatusToSerial("AP_ALTITUDE", currentAltitudeHold > 0.5);
    lastSentAltitudeHold = currentAltitudeHold;

    sendModeStatusToSerial("AP_VS", currentVsHold > 0.5);
    lastSentVsHold = currentVsHold;

    sendModeStatusToSerial("AP_SPEED", currentAirspeedHold > 0.5);
    lastSentAirspeedHold = currentAirspeedHold;

    sendModeStatusToSerial("AP_VNAV", currentFlc > 0.5);
    lastSentFlc = currentFlc;
}

void AutopilotDataReader::sendToSerial(const std::string& type, double value) {
    if (!serialPort.isConnected()) {
        return;
    }

    // Convert to integer and format: Type Value\n (e.g., H 120, VS 500, A 5000, S 250)
    int intValue = static_cast<int>(std::round(value));
    std::string message = type + " " + std::to_string(intValue) + "\n";

    // Write to serial port
    DWORD bytesWritten;
    HANDLE hSerial = serialPort.getHandle();

    if (!WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to serial port" << std::endl;
    } else {
        std::cout << "Sent to serial: " << type << " = " << intValue << std::endl;
    }
}

void AutopilotDataReader::sendApStatusToSerial(bool isOn) {
    if (!serialPort.isConnected()) {
        return;
    }

    // Format: AP ON\n or AP OFF\n
    std::string message = isOn ? "AP ON\n" : "AP OFF\n";

    // Write to serial port
    DWORD bytesWritten;
    HANDLE hSerial = serialPort.getHandle();

    if (!WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to serial port" << std::endl;
    } else {
        std::cout << "Sent to serial: AP " << (isOn ? "ON" : "OFF") << std::endl;
    }
}

void AutopilotDataReader::sendModeStatusToSerial(const std::string& mode, bool isOn) {
    if (!serialPort.isConnected()) {
        return;
    }

    // Format: AP_HEADING ON\n or AP_HEADING OFF\n
    std::string message = mode + (isOn ? " ON\n" : " OFF\n");

    // Write to serial port
    DWORD bytesWritten;
    HANDLE hSerial = serialPort.getHandle();

    if (!WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to serial port" << std::endl;
    } else {
        std::cout << "Sent to serial: " << mode << " " << (isOn ? "ON" : "OFF") << std::endl;
    }
}

bool AutopilotDataReader::hasChanged(double current, double last, double threshold) {
    // First time sending (last == -999.0)
    if (last < -900.0) {
        return true;
    }

    // Check if difference exceeds threshold
    return std::abs(current - last) >= threshold;
}
