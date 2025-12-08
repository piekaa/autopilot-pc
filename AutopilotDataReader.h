#ifndef AUTOPILOTDATAREADER_H
#define AUTOPILOTDATAREADER_H

#include <Windows.h>
#include <SimConnect.h>
#include <string>
#include <chrono>
#include "SerialPortReader.h"

/**
 * AutopilotDataReader class (Java-style)
 * Reads autopilot values from MSFS and sends them to serial port
 */
class AutopilotDataReader {
private:
    HANDLE hSimConnect;
    SerialPortReader& serialPort;

    // Current values
    double currentHeading;
    double currentVerticalSpeed;
    double currentAltitude;
    double currentSpeed;

    // Last sent values (for change detection)
    double lastSentHeading;
    double lastSentVerticalSpeed;
    double lastSentAltitude;
    double lastSentSpeed;

    bool dataReceived;

    // Timing for periodic updates
    std::chrono::steady_clock::time_point lastFullUpdate;
    std::chrono::steady_clock::time_point lastRequest;

    // SimConnect IDs
    enum DATA_DEFINE_ID {
        DEFINITION_AUTOPILOT_DATA
    };

    enum DATA_REQUEST_ID {
        REQUEST_AUTOPILOT_DATA
    };

    // Struct to receive all autopilot data at once
    struct AutopilotData {
        double heading;
        double verticalSpeed;
        double altitude;
        double speed;
    };

    // Helper methods
    void sendToSerial(const std::string& type, double value);
    bool hasChanged(double current, double last, double threshold = 0.5);

public:
    // Constructor
    AutopilotDataReader(HANDLE simConnectHandle, SerialPortReader& serial);

    // Initialize - set up data definitions
    bool initialize();

    // Request data from simulator
    void requestData();

    // Process SimConnect messages
    void processMessage(SIMCONNECT_RECV* pData);

    // Update - call this regularly (handles timing and change detection)
    void update();

    // Send all values regardless of changes
    void sendAllValues();
};

#endif // AUTOPILOTDATAREADER_H
