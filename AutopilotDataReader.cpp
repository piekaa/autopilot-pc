#include "AutopilotDataReader.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

AutopilotDataReader::AutopilotDataReader(HANDLE simConnectHandle, SerialPortReader& serial)
    : hSimConnect(simConnectHandle), serialPort(serial),
      currentHeading(0.0), currentVerticalSpeed(0.0), currentAltitude(0.0), currentSpeed(0.0), currentApMaster(0.0),
      lastSentHeading(-999.0), lastSentVerticalSpeed(-999.0), lastSentAltitude(-999.0), lastSentSpeed(-999.0), lastSentApMaster(-999.0),
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

bool AutopilotDataReader::hasChanged(double current, double last, double threshold) {
    // First time sending (last == -999.0)
    if (last < -900.0) {
        return true;
    }

    // Check if difference exceeds threshold
    return std::abs(current - last) >= threshold;
}
