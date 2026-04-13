#ifndef MSFS_CONTROLLER_MSFSAUTOPILOTWRITER_H
#define MSFS_CONTROLLER_MSFSAUTOPILOTWRITER_H

#include "AutopilotWriter.h"
#include "../sdk/SdkWriteConnection.h"

class MSFSAutopilotWriter : public AutopilotWriter {
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
    MSFSAutopilotWriter(HANDLE* connection, std::unordered_map<std::string, unsigned long long> inputEvents) {
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

    void setHeading(int headingValue) override {
        heading->value = headingValue;
        SdkWriteConnection::setValue(connection, heading);
    }

    void setAltitudeIndex() override {
        SdkWriteConnection::setValue(connection, altitudeIndex);
    }

    void setAltitude(int altitudeValue) override {
        altitude->value = altitudeValue;

        SdkWriteConnection::setValue(connection, altitude);
    }

    void setSpeed(int speedValue) override {
        speed->value = speedValue;
        SdkWriteConnection::setValue(connection, speed);
    }

    void setVerticalSpeed(int verticalSpeedValue) override {
        verticalSpeed->value = verticalSpeedValue;
        SdkWriteConnection::setValue(connection, verticalSpeed);
    }

    void toggleAutopilot() override {
        SdkWriteConnection::setValue(connection, autopilotToggle);
    }

    void toggleHeading() override {
        SdkWriteConnection::setValue(connection, headingToggle);
    }

    void toggleSpeed() override {
        SdkWriteConnection::setValue(connection, autoThrottleToggle);
    }

    void toggleAltitude() override {
        SdkWriteConnection::setValue(connection, altitudeHoldToggle);
    }

    void toggleVerticalSpeed() override {
        SdkWriteConnection::setValue(connection, verticalSpeedToggle);
    }

    void toggleLNav() override {
        SdkWriteConnection::setValue(connection, lNavToggle);
    }

    void toggleVNav() override {
        SdkWriteConnection::setValue(connection, vNavToggle);
    }

    virtual ~MSFSAutopilotWriter() {
        delete heading;
    }

    bool working() override {
        return false;
    }
};

#endif //MSFS_CONTROLLER_MSFSAUTOPILOTWRITER_H
