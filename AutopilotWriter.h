#ifndef MSFS_CONTROLLER_AUTOPILOTWRITER_H
#define MSFS_CONTROLLER_AUTOPILOTWRITER_H

#include "SdkWriteConnection.h"

class AutopilotWriter {
protected:
    HANDLE* connection;
    AutopilotWriteField* heading = new AutopilotWriteField("HEADING_BUG_SET");
    AutopilotWriteField* altitude = new AutopilotWriteField("AP_ALT_VAR_SET_ENGLISH");
    AutopilotWriteField* altitudeIndex = new AutopilotWriteField("ALTITUDE_SLOT_INDEX_SET");
    AutopilotWriteField* autoThrottleToggle = new AutopilotWriteField("AUTO_THROTTLE_ARM");

    std::unordered_map<std::string, unsigned long long> inputEvents;

public:
    AutopilotWriter(HANDLE* connection, std::unordered_map<std::string, unsigned long long> inputEvents) {
        this->connection = connection;
        SdkWriteConnection::registerField(connection, heading);
        SdkWriteConnection::registerField(connection, altitude);
        SdkWriteConnection::registerField(connection, altitudeIndex);
        SdkWriteConnection::registerField(connection, autoThrottleToggle);
        this->inputEvents = inputEvents;

        // for (const auto& [eventName, eventId] : inputEvents) {
        //     std::cout << "Registering event " << eventName << " with id " << eventId << std::endl;
        // }

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

    virtual void toggleSpeed() {
        SdkWriteConnection::setValue(connection, autoThrottleToggle);
    }

    virtual ~AutopilotWriter() {
        delete heading;
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITER_H