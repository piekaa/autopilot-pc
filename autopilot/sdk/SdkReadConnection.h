#ifndef MSFS_CONTROLLER_SIMCONNECT_H
#define MSFS_CONTROLLER_SIMCONNECT_H
#include <Windows.h>
#include <SimConnect.h>
#include <iostream>
#include <thread>
#include <unordered_map>


class SdkReadConnection {
public:
    static void registerForAircraftName(HANDLE *connection) {
        auto hr = SimConnect_AddToDataDefinition(*connection,
                                                 1,
                                                 "TITLE",
                                                 nullptr,
                                                 SIMCONNECT_DATATYPE_STRING256);
        if (hr != S_OK) {
            std::cerr << "Failed to map TITLE event" << std::endl;
        }
    }

    static void registerAutopilotField(HANDLE *connection, std::string fieldName, std::string units) {
        auto hr = SimConnect_AddToDataDefinition(*connection,
                                                 0,
                                                 fieldName.c_str(),
                                                 units.c_str(),
                                                 SIMCONNECT_DATATYPE_FLOAT64);
        if (hr != S_OK) {
            std::cerr << "Failed to map " << fieldName << " event" << std::endl;
        }
    }

    static AutopilotValues *readAutopilotData(HANDLE *connection) {
        DWORD cbData;
        SIMCONNECT_RECV *pData;
        if (SUCCEEDED(SimConnect_GetNextDispatch(*connection, &pData, &cbData))) {
        } else {
            return nullptr;
        }


        switch (pData->dwID) {
            case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
                auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

                if (pObjData->dwRequestID == 0) {
                    auto *autopilotData = (AutopilotValues *) &pObjData->dwData;

                    return new AutopilotValues(*autopilotData);
                }
                break;
            }

            case SIMCONNECT_RECV_ID_EXCEPTION: {
                auto *pException = (SIMCONNECT_RECV_EXCEPTION *) pData;
                std::cout << "SimConnect exception in AutopilotDataReader: " << pException->dwException << std::endl;
                break;
            }

            case SIMCONNECT_RECV_ID_OPEN: {
                break;
            }

            default: {
                std::cout << "Unexpected type of message in readAutopilotData: " << pData->dwID << std::endl;
                break;
            }
        }

        return nullptr;
    }

    static std::unordered_map<std::string, unsigned long long> readEnumerations(HANDLE *connection) {
        std::unordered_map<std::string, unsigned long long> eventMap;

        while (true) {
            DWORD cbData;
            SIMCONNECT_RECV *pData;

            if (SUCCEEDED(SimConnect_GetNextDispatch(*connection, &pData, &cbData))) {
                switch (pData->dwID) {
                    case SIMCONNECT_RECV_ID_ENUMERATE_INPUT_EVENTS: {
                        // Handle InputEvent enumeration responses
                        auto data = (SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS *) pData;

                        // Process enumerated InputEvents
                        std::cout << "Processing InputEvent enumeration batch (RequestID: " << data->dwRequestID
                                << ", Entry count: " << data->dwEntryNumber << "/" << data->dwOutOf << ")" << std::endl;

                        // Iterate through all InputEvent descriptors in this batch
                        for (DWORD i = 0; i < data->dwArraySize; i++) {
                            SIMCONNECT_INPUT_EVENT_DESCRIPTOR *descriptor = &data->rgData[i];
                            eventMap[descriptor->Name] = descriptor->Hash;
                        }

                        // Return when we've received all batches
                        if (data->dwEntryNumber >= data->dwOutOf - 1) {
                            return eventMap;
                        }
                        break;
                    }
                    default:
                        // Ignore other message types and continue polling
                        break;
                }
            } else {
                // No message available, sleep briefly to avoid busy-waiting
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    static void requestAutopilotData(HANDLE *connection) {
        HRESULT result = SimConnect_RequestDataOnSimObject(
            *connection,
            0,
            0,
            SIMCONNECT_OBJECT_ID_USER,
            SIMCONNECT_PERIOD_ONCE
        );

        if (result != S_OK) {
            std::cerr << "Failed to request autopilot data" << std::endl;
        }
    }

    static void requestEnumerateInputEvents(HANDLE *connection) {
        auto result = SimConnect_EnumerateInputEvents(*connection, 1000);
        if (result != S_OK) {
            std::cerr << "Warning: Failed to enumerate InputEvents (this is normal if aircraft doesn't support it)" <<
                    std::endl;
        }
    }

    static char *readAircraftName(HANDLE *connection) {
        DWORD cbData;
        SIMCONNECT_RECV *pData;
        if (SUCCEEDED(SimConnect_GetNextDispatch(*connection, &pData, &cbData))) {
        } else {
            return nullptr;
        }

        switch (pData->dwID) {
            case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
                auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

                if (pObjData->dwRequestID == 1) {
                    auto *aircraftName = (char *) &pObjData->dwData;
                    return aircraftName;
                }
                break;
            }

            case SIMCONNECT_RECV_ID_EXCEPTION: {
                auto *pException = (SIMCONNECT_RECV_EXCEPTION *) pData;
                std::cout << "SimConnect exception in readAircraftName: " << pException->dwException << std::endl;
                break;
            }

            case SIMCONNECT_RECV_ID_OPEN: {
                break;
            }

            default: {
                std::cout << "Unexpected type of message in readAircraftName: " << pData->dwID << std::endl;
                break;
            }
        }

        return nullptr;
    }

    static void requestAircraftName(HANDLE *connection) {
        HRESULT hr = SimConnect_RequestDataOnSimObject(
            *connection,
            1,
            1,
            SIMCONNECT_OBJECT_ID_USER,
            SIMCONNECT_PERIOD_ONCE
        );

        if (hr != S_OK) {
            std::cerr << "Failed to request aircraft name" << std::endl;
        }
    }
};

#endif //MSFS_CONTROLLER_SIMCONNECT_H
