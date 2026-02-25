#ifndef MSFS_CONTROLLER_AUTOPILOTWRITER_H
#define MSFS_CONTROLLER_AUTOPILOTWRITER_H

#include "SdkWriteConnection.h"

class AutopilotWriter {
    HANDLE* connection;

    AutopilotWriteField* heading = new AutopilotWriteField("HEADING_BUG_SET");

public:
    AutopilotWriter(HANDLE* connection) {
        this->connection = connection;
        SdkWriteConnection::registerField(connection, heading);
    }

    void setHeading(int headingValue) {
        heading->value = headingValue;
        SdkWriteConnection::setValue(connection, heading);
    }

    ~AutopilotWriter() {
        delete heading;
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITER_H