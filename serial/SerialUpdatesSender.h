#ifndef MSFS_CONTROLLER_SERIALUPDATESSENDER_H
#define MSFS_CONTROLLER_SERIALUPDATESSENDER_H
#include "../autopilot/rw/AutopilotValues.h"
#include "Serial.h"

class SerialUpdatesSender {
    Serial* serial;
    AutopilotValues lastValues;
    int i = 0;
public:

    SerialUpdatesSender(Serial* serial) {
        this->serial = serial;
        lastValues = AutopilotValues();
        lastValues.altitude = 0.5;
    }

    void sendIfNeeded(AutopilotValues values) {
        if (i % 10 == 0) {
            serial->write(values.toCommunicationString());
        } else {
            if (values != lastValues) {
                serial->write(lastValues.toCommunicationString());
            }
        }
        lastValues = values;
        i++;
    }
};

#endif //MSFS_CONTROLLER_SERIALUPDATESSENDER_H