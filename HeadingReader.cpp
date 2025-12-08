#include "HeadingReader.h"
#include <iostream>

HeadingReader::HeadingReader(HANDLE simConnectHandle)
    : hSimConnect(simConnectHandle), currentHeading(0.0), dataReceived(false) {
    // Constructor
}

bool HeadingReader::initialize() {
    HRESULT hr;

    // Define what data we want to receive (similar to defining a data structure in Java)
    hr = SimConnect_AddToDataDefinition(
        hSimConnect,
        DEFINITION_HEADING,
        "AUTOPILOT HEADING LOCK DIR",  // The SimVar name
        "degrees",                       // Unit
        SIMCONNECT_DATATYPE_FLOAT64     // Data type
    );

    if (hr != S_OK) {
        std::cerr << "Failed to add data definition. Error: 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    std::cout << "HeadingReader initialized successfully." << std::endl;
    return true;
}

void HeadingReader::requestHeading() {
    // Request data from SimConnect (like making an API call in Java)
    HRESULT hr = SimConnect_RequestDataOnSimObject(
        hSimConnect,
        REQUEST_HEADING,
        DEFINITION_HEADING,
        SIMCONNECT_OBJECT_ID_USER,  // User's aircraft
        SIMCONNECT_PERIOD_ONCE      // Request once
    );

    if (hr != S_OK) {
        std::cerr << "Failed to request heading data. Error: 0x" << std::hex << hr << std::dec << std::endl;
    }
}

double HeadingReader::getHeading() const {
    return currentHeading;
}

bool HeadingReader::hasDataReceived() const {
    return dataReceived;
}

void HeadingReader::processMessage(SIMCONNECT_RECV* pData) {
    // Process messages (like handling callbacks in Java)
    switch (pData->dwID) {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
            auto* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            if (pObjData->dwRequestID == REQUEST_HEADING) {
                auto* headingData = (HeadingData*)&pObjData->dwData;
                currentHeading = headingData->heading;
                dataReceived = true;

                std::cout << "Autopilot Heading: " << currentHeading << " degrees" << std::endl;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_EXCEPTION: {
            auto* pException = (SIMCONNECT_RECV_EXCEPTION*)pData;
            std::cerr << "SimConnect exception received. Exception ID: " << pException->dwException << std::endl;
            break;
        }

        default:
            break;
    }
}

void CALLBACK HeadingReader::dispatchCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
    // Static callback function that forwards to instance method
    auto* reader = static_cast<HeadingReader*>(pContext);
    if (reader) {
        reader->processMessage(pData);
    }
}
