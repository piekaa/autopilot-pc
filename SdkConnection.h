#ifndef MSFS_CONTROLLER_SIMCONNECT_H
#define MSFS_CONTROLLER_SIMCONNECT_H
#include <Windows.h>
#include <SimConnect.h>
#include <iostream>
#include <thread>
#include <chrono>


class SdkConnection {
public:
    static void registerAutopilotField(HANDLE *connection, std::string fieldName, std::string units) {
        auto hr = SimConnect_AddToDataDefinition(*connection,
                                                 0,
                                                 fieldName.c_str(),
                                                 units.c_str());
        if (hr != S_OK) {
            std::cerr << "Failed to map " << fieldName << " event" << std::endl;
        }
    }
    static AutopilotValues* readAutopilot(HANDLE *connection) {
        HRESULT hr = SimConnect_RequestDataOnSimObject(
            *connection,
            0,
            0,
            SIMCONNECT_OBJECT_ID_USER,
            SIMCONNECT_PERIOD_ONCE
        );

        if (hr != S_OK) {
            std::cerr << "Failed to request autopilot data" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        DWORD cbData;
        SIMCONNECT_RECV *pData;
        if (SUCCEEDED(SimConnect_GetNextDispatch(*connection, &pData, &cbData))) {
            std::cout << "Did read successfuly next dispatch" << std::endl;
        } else {
            std::cout << "Failed to read next dispach" << std::endl;
            return nullptr;
        }


        switch (pData->dwID) {
            case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
                std::cout << "Autopilot data received" << std::endl;
                auto *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *) pData;

                if (pObjData->dwRequestID == 0) {
                    auto* autopilotData = (AutopilotValues*)&pObjData->dwData;

                    return autopilotData;
                }
                break;
            }

            case SIMCONNECT_RECV_ID_EXCEPTION: {
                auto *pException = (SIMCONNECT_RECV_EXCEPTION *) pData;
                std::cout << "SimConnect exception in AutopilotDataReader: " << pException->dwException << std::endl;
                break;
            }

            default: {
                std::cout << "Unexpected type of message in registerAutopilotField: " << pData->dwID << std::endl;
                break;
            }
        }

        return nullptr;
    }
};

#endif //MSFS_CONTROLLER_SIMCONNECT_H
