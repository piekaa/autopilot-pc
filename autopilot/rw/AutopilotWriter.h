#ifndef MSFS_CONTROLLER_AUTOPILOTWRITER_H
#define MSFS_CONTROLLER_AUTOPILOTWRITER_H

#include "../sdk/SdkWriteConnection.h"

class AutopilotWriter {
protected:
    HANDLE* connection;
    AutopilotWriteField* heading = new AutopilotWriteField("HEADING_BUG_SET");
    AutopilotWriteField* speed = new AutopilotWriteField("AP_SPD_VAR_SET");
    AutopilotWriteField* altitude = new AutopilotWriteField("AP_ALT_VAR_SET_ENGLISH");
    AutopilotWriteField* verticalSpeed = new AutopilotWriteField("AP_VS_VAR_SET_ENGLISH");
    AutopilotWriteField* altitudeIndex = new AutopilotWriteField("ALTITUDE_SLOT_INDEX_SET");
    AutopilotWriteField* autopilotToggle = new AutopilotWriteField("AP_MASTER");
    AutopilotWriteField* headingToggle = new AutopilotWriteField("AP_HDG_HOLD");
    AutopilotWriteField* autoThrottleToggle = new AutopilotWriteField("AUTO_THROTTLE_ARM");
    AutopilotWriteField* altitudeHoldToggle = new AutopilotWriteField("AP_ALT_HOLD");
    AutopilotWriteField* verticalSpeedToggle = new AutopilotWriteField("AP_VS_HOLD");
    AutopilotWriteField* lNavToggle = new AutopilotWriteField("AP_NAV1_HOLD");
    AutopilotWriteField* vNavToggle = new AutopilotWriteField("FLIGHT_LEVEL_CHANGE");

    std::unordered_map<std::string, unsigned long long> inputEvents;

public:
    AutopilotWriter(HANDLE* connection, std::unordered_map<std::string, unsigned long long> inputEvents) {
        this->connection = connection;
        SdkWriteConnection::registerField(connection, heading);
        SdkWriteConnection::registerField(connection, speed);
        SdkWriteConnection::registerField(connection, altitude);
        SdkWriteConnection::registerField(connection, verticalSpeed);
        SdkWriteConnection::registerField(connection, altitudeIndex);
        SdkWriteConnection::registerField(connection, autopilotToggle);
        SdkWriteConnection::registerField(connection, headingToggle);
        SdkWriteConnection::registerField(connection, autoThrottleToggle);
        SdkWriteConnection::registerField(connection, altitudeHoldToggle);
        SdkWriteConnection::registerField(connection, verticalSpeedToggle);
        SdkWriteConnection::registerField(connection, lNavToggle);
        SdkWriteConnection::registerField(connection, vNavToggle);
        this->inputEvents = inputEvents;
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

    virtual void setSpeed(int speedValue) {
        speed->value = speedValue;
        SdkWriteConnection::setValue(connection, speed);
    }

    virtual void setVerticalSpeed(int verticalSpeedValue) {
        verticalSpeed->value = verticalSpeedValue;
        SdkWriteConnection::setValue(connection, verticalSpeed);
    }

    virtual void toggleAutopilot() {
        SdkWriteConnection::setValue(connection, autopilotToggle);
    }

    virtual void toggleHeading() {
        SdkWriteConnection::setValue(connection, headingToggle);
    }

    virtual void toggleSpeed() {
        SdkWriteConnection::setValue(connection, autoThrottleToggle);
    }

    virtual void toggleAltitude() {
        SdkWriteConnection::setValue(connection, altitudeHoldToggle);
    }

    virtual void toggleVerticalSpeed() {
        SdkWriteConnection::setValue(connection, verticalSpeedToggle);
    }

    virtual void toggleLNav() {
        SdkWriteConnection::setValue(connection, lNavToggle);
    }

    virtual void toggleVNav() {
        SdkWriteConnection::setValue(connection, vNavToggle);
    }

    virtual ~AutopilotWriter() {
        delete heading;
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITER_H