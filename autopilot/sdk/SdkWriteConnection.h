#ifndef MSFS_CONTROLLER_SDKWRITECONNECTION_H
#define MSFS_CONTROLLER_SDKWRITECONNECTION_H
#include "../rw/AutopilotWriteField.h"
#include <iostream>

#include "DoubleValue.h"
#include "SdkConnectionMutex.h"

class SdkWriteConnection {
public:
    static void registerField(HANDLE *connection, AutopilotWriteField *autopilotWriteField) {
        std::lock_guard<std::mutex> lock(SdkConnectionMutex::mtx);
        auto hr = SimConnect_MapClientEventToSimEvent(*connection, autopilotWriteField->getId(),
                                                      autopilotWriteField->getEventName().c_str());
        if (hr != S_OK) {
            std::cout << "Failed to map " << autopilotWriteField->getEventName() << " event" << std::endl;
        }
    }

    static void setValue(HANDLE *connection, AutopilotWriteField *autopilotWriteField) {
        std::lock_guard<std::mutex> lock(SdkConnectionMutex::mtx);
        HRESULT hr;

        if (autopilotWriteField->isDoubleValued()) {
            hr = SimConnect_TransmitClientEvent_EX1(*connection, 0, autopilotWriteField->getId(),
                                                    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                                    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY,
                                                    autopilotWriteField->value, autopilotWriteField->getValue2());
        } else {
            hr = SimConnect_TransmitClientEvent(*connection, 0, autopilotWriteField->getId(),
                                                autopilotWriteField->value, SIMCONNECT_GROUP_PRIORITY_HIGHEST,
                                                SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
        }


        if (hr != S_OK) {
            std::cout << "Failed to set " << autopilotWriteField->getEventName() << " value" << std::endl;
        }
    }

    static void sendInputEvent(HANDLE *connection, unsigned long long hash, double value) {
        std::lock_guard<std::mutex> lock(SdkConnectionMutex::mtx);
        auto result = SimConnect_SetInputEvent(*connection, hash, sizeof(value), &value);
        if (result != S_OK) {
            std::cerr << "Failed to trigger FCC_SPEED InputEvent (HRESULT: " << result << ")" << std::endl;
        }
    }

    static void registerSimVar(HANDLE *connection, AutopilotWriteField *autopilotWriteField) {
        std::lock_guard<std::mutex> lock(SdkConnectionMutex::mtx);
        SimConnect_AddToDataDefinition(
            *connection,
            autopilotWriteField->getId(), // Unique definition ID
            autopilotWriteField->getEventName().c_str(), // SimVar name
            autopilotWriteField->getUnits().c_str(), // Units
            SIMCONNECT_DATATYPE_FLOAT64
        );
    }

    static void setSimVar(HANDLE *connection, AutopilotWriteField *autopilotWriteField) {
        std::lock_guard<std::mutex> lock(SdkConnectionMutex::mtx);

        DoubleValue doubleValue{};
        doubleValue.value = autopilotWriteField->value;

        SimConnect_SetDataOnSimObject(
            *connection,
            autopilotWriteField->getId(),  // Same definition ID from step 2
            SIMCONNECT_OBJECT_ID_USER,  // Target object (user aircraft)
            0,  // Flags (usually 0)
            0,  // Array count (0 for single object)
            sizeof(doubleValue),  // Size of data structure
            &doubleValue // Pointer to your data
        );
    }
};

#endif //MSFS_CONTROLLER_SDKWRITECONNECTION_H
