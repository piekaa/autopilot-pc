#ifndef MSFS_CONTROLLER_VISIONJETWRITER_H
#define MSFS_CONTROLLER_VISIONJETWRITER_H

#include <chrono>
#include <thread>
#include <unordered_map>
#include <windows.h>
#include "../autopilot/rw/MSFSAutopilotWriter.h"

class VisionJetWriter: public MSFSAutopilotWriter {
public:
    VisionJetWriter(HANDLE *connection,
                       std::unordered_map<std::string, unsigned long long> inputEvents) : MSFSAutopilotWriter(
        connection, inputEvents) {
    }

    void toggleSpeed() override {
        SdkWriteConnection::sendInputEvent(connection, inputEvents["SF50_AUTOPILOT_MAN_SPEED_MODE"], 1.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        MSFSAutopilotWriter::toggleSpeed();
    }
};

#endif //MSFS_CONTROLLER_VISIONJETWRITER_H