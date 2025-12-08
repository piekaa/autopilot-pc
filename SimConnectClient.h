#ifndef SIMCONNECTCLIENT_H
#define SIMCONNECTCLIENT_H

#include <Windows.h>
#include <SimConnect.h>
#include <string>

/**
 * SimConnect client wrapper class (Java-style)
 * Handles connection and disconnection to MSFS
 */
class SimConnectClient {
private:
    HANDLE hSimConnect;
    bool connected;

public:
    // Constructor
    SimConnectClient();

    // Destructor
    ~SimConnectClient();

    // Connect to MSFS
    bool connect(const std::string& appName);

    // Disconnect from MSFS
    void disconnect();

    // Check if connected
    bool isConnected() const;

    // Get the SimConnect handle
    HANDLE getHandle() const;

    // Process messages from SimConnect
    void processMessages();
};

#endif // SIMCONNECTCLIENT_H
