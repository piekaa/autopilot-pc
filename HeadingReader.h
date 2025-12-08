#ifndef HEADINGREADER_H
#define HEADINGREADER_H

#include <Windows.h>
#include <SimConnect.h>

/**
 * HeadingReader class (Java-style)
 * Responsible for reading autopilot heading from MSFS
 */
class HeadingReader {
private:
    HANDLE hSimConnect;
    double currentHeading;
    bool dataReceived;

    // SimConnect IDs (like constants in Java)
    enum DATA_DEFINE_ID {
        DEFINITION_HEADING
    };

    enum DATA_REQUEST_ID {
        REQUEST_HEADING
    };

    // Struct to receive heading data
    struct HeadingData {
        double heading;
    };

public:
    // Constructor
    HeadingReader(HANDLE simConnectHandle);

    // Initialize - set up data definitions
    bool initialize();

    // Request heading data
    void requestHeading();

    // Get the current heading value
    double getHeading() const;

    // Check if data has been received
    bool hasDataReceived() const;

    // Process SimConnect messages for this reader
    void processMessage(SIMCONNECT_RECV* pData);

    // Static callback for SimConnect
    static void CALLBACK dispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);
};

#endif // HEADINGREADER_H
