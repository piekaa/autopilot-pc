#ifndef MSFS_CONTROLLER_SIMCONNECT_H
#define MSFS_CONTROLLER_SIMCONNECT_H
#include "SimConnectReadonlyField.h"
#include <Windows.h>
#include <SimConnect.h>
#include <iostream>


class SimConnect {

    static void registerField(HANDLE* connection, SimConnectReadonlyField* field) {
        auto hr = SimConnect_AddToDataDefinition(*connection,
            0,
            field->getFieldName().c_str(),
            field->getUnits().c_str());
        if (hr != S_OK) {
            std::cerr << "Failed to map " << field->getFieldName() << " event" << std::endl;
        }
    }

public:
    static SimConnectReadonlyField* registerField(HANDLE* connection, std::string fieldName, std::string units) {
        auto field = new SimConnectReadonlyField(fieldName, units);
        registerField(connection, field);
        return field;
    }
};

#endif //MSFS_CONTROLLER_SIMCONNECT_H