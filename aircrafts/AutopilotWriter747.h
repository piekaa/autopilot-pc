#ifndef MSFS_CONTROLLER_AUTOPILORWRITER737_H
#define MSFS_CONTROLLER_AUTOPILORWRITER737_H

#include <windows.h>
#include "../autopilot/rw/MSFSAutopilotWriter.h"

class AutopilotWriter737 : public MSFSAutopilotWriter {
public:
    unsigned long long fccSpeedHash;

    AutopilotWriter737(HANDLE *connection,
                       std::unordered_map<std::string, unsigned long long> inputEvents) : MSFSAutopilotWriter(
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
        MSFSAutopilotWriter::setAltitude(altitudeValue);
    }

    void toggleSpeed() override {
        MSFSAutopilotWriter::toggleSpeed();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        SdkWriteConnection::sendInputEvent(connection, inputEvents["FCC_SPEED"], 1.0);
    }

};

#endif //MSFS_CONTROLLER_AUTOPILORWRITER737_H
