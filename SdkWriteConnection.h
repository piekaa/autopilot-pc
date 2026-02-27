#ifndef MSFS_CONTROLLER_SDKWRITECONNECTION_H
#define MSFS_CONTROLLER_SDKWRITECONNECTION_H
#include "AutopilotWriteField.h"
#include <iostream>

class SdkWriteConnection {
public:
    static void registerField(HANDLE *connection, AutopilotWriteField* autopilotWriteField) {
        auto hr = SimConnect_MapClientEventToSimEvent(*connection, autopilotWriteField->getId(), autopilotWriteField->getEventName().c_str());
        if (hr != S_OK) {
            std::cout << "Failed to map " << autopilotWriteField->getEventName() << " event" << std::endl;
        }
    }

    static void setValue(HANDLE *connection, AutopilotWriteField* autopilotWriteField) {
        HRESULT hr;

        if (autopilotWriteField->isDoubleValued()) {
            hr = SimConnect_TransmitClientEvent_EX1(*connection, 0, autopilotWriteField->getId(),
                                       SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY, autopilotWriteField->value, autopilotWriteField->getValue2());
        } else {
            hr = SimConnect_TransmitClientEvent(*connection, 0, autopilotWriteField->getId(),
          autopilotWriteField->value, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
        }


        if (hr != S_OK) {
            std::cout << "Failed to set " << autopilotWriteField->getEventName() << " value" << std::endl;
        }
    }
};

#endif //MSFS_CONTROLLER_SDKWRITECONNECTION_H