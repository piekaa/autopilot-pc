#ifndef MSFS_CONTROLLER_AUTOPILOTREADER_H
#define MSFS_CONTROLLER_AUTOPILOTREADER_H

#include <Windows.h>
#include <SimConnect.h>

#include "AutopilotValues.h"
#include "SdkConnection.h"

class AutopilotReader {
    HANDLE* connection;
public:
    AutopilotReader(HANDLE* connection) {
        this->connection = connection;
        SdkConnection::registerAutopilotField(connection, "AUTOPILOT HEADING LOCK DIR", "degrees");
    }

    AutopilotValues* read() {
        return SdkConnection::readAutopilot(connection);
    }
};


#endif //MSFS_CONTROLLER_AUTOPILOTREADER_H