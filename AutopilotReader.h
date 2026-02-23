#ifndef MSFS_CONTROLLER_AUTOPILOTREADER_H
#define MSFS_CONTROLLER_AUTOPILOTREADER_H

#include <Windows.h>
#include <SimConnect.h>

#include "SimConnect.h"
#include "SimConnectReadonlyField.h"

class AutopilotReader {
    HANDLE* connection;
public:
    AutopilotReader(HANDLE* connection) {
        this->connection = connection;

        auto heading = SimConnect::registerField(connection, "AUTOPILOT HEADING LOCK DIR", "degrees");
    }
};


#endif //MSFS_CONTROLLER_AUTOPILOTREADER_H