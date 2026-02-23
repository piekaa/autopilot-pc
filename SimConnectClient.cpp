#include "SimConnectClient.h"
#include <iostream>
#include <SimConnect.h>

SimConnectClient::SimConnectClient() : hSimConnect(nullptr), connected(false) {
    // Constructor - initialize member variables
}

SimConnectClient::~SimConnectClient() {
    // Destructor - clean up
    disconnect();
}

bool SimConnectClient::connect(const std::string& appName) {
    if (connected) {
        std::cout << "Already connected!" << std::endl;
        return true;
    }

    HRESULT hr = SimConnect_Open(
        &hSimConnect,
        appName.c_str(),
        nullptr,
        0,
        0,
        0
    );

    if (hr == S_OK) {
        connected = true;
        std::cout << "Successfully connected to MSFS!" << std::endl;
        return true;
    } else {
        std::cerr << "Failed to connect to MSFS. Is the simulator running?" << std::endl;
        std::cerr << "Error code: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }
}

void SimConnectClient::disconnect() {
    if (connected && hSimConnect != nullptr) {
        SimConnect_Close(hSimConnect);
        hSimConnect = nullptr;
        connected = false;
        std::cout << "Disconnected from MSFS." << std::endl;
    }
}

bool SimConnectClient::isConnected() const {
    return connected;
}

HANDLE SimConnectClient::getHandle() const {
    return hSimConnect;
}

void SimConnectClient::processMessages() {
    if (!connected) {
        return;
    }

    DWORD cbData;
    SIMCONNECT_RECV* pData;

    while (SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData) == S_OK) {
        // Messages are processed by callbacks registered elsewhere
        // This just pumps the message queue
    }
}
