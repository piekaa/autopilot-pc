#ifndef MSFS_CONTROLLER_AUTOPILORWRITER737_H
#define MSFS_CONTROLLER_AUTOPILORWRITER737_H

#include <windows.h>
#include "../autopilot/rw/AutopilotWriter.h"

class AutopilotWriter737 : public AutopilotWriter {
public:
    unsigned long long fccSpeedHash;

    AutopilotWriter737(HANDLE *connection,
                       std::unordered_map<std::string, unsigned long long> inputEvents) : AutopilotWriter(
        connection, inputEvents) {
        altitudeIndex->value = 3;
        for (const auto &[eventName, hash]: inputEvents) {
            if (eventName == "FCC_SPEED") {
                fccSpeedHash = hash;
            }
        }
    }

    void setAltitude(int altitudeValue) override {
        altitude->setValue2(3);
        AutopilotWriter::setAltitude(altitudeValue);
    }

    void toggleSpeed() override {
        AutopilotWriter::toggleSpeed();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        SdkWriteConnection::sendInputEvent(connection, inputEvents["FCC_SPEED"], 1.0);
    }

};

#endif //MSFS_CONTROLLER_AUTOPILORWRITER737_H
