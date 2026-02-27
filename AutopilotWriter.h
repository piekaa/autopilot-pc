#ifndef MSFS_CONTROLLER_AUTOPILOTWRITER_H
#define MSFS_CONTROLLER_AUTOPILOTWRITER_H

#include "SdkWriteConnection.h"

class AutopilotWriter {
    HANDLE* connection;

protected:
    AutopilotWriteField* heading = new AutopilotWriteField("HEADING_BUG_SET");
    AutopilotWriteField* altitude = new AutopilotWriteField("AP_ALT_VAR_SET_ENGLISH");
    AutopilotWriteField* altitudeIndex = new AutopilotWriteField("ALTITUDE_SLOT_INDEX_SET");

public:
    AutopilotWriter(HANDLE* connection) {
        this->connection = connection;
        SdkWriteConnection::registerField(connection, heading);
        SdkWriteConnection::registerField(connection, altitude);
        SdkWriteConnection::registerField(connection, altitudeIndex);
    }

    virtual void setHeading(int headingValue) {
        heading->value = headingValue;
        SdkWriteConnection::setValue(connection, heading);
    }

    void setAltitudeIndex() {
        SdkWriteConnection::setValue(connection, altitudeIndex);
    }

    virtual void setAltitude(int altitudeValue) {
        altitude->value = altitudeValue;

        SdkWriteConnection::setValue(connection, altitude);
    }

    virtual ~AutopilotWriter() {
        delete heading;
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITER_H