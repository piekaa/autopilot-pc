#ifndef MSFS_CONTROLLER_AIRBUS_WRITER_H
#define MSFS_CONTROLLER_AIRBUS_WRITER_H

#include <unordered_map>
#include <windows.h>
#include <thread>
#include <atomic>
#include "../autopilot/rw/MSFSAutopilotWriter.h"

class AirbusWriter : public MSFSAutopilotWriter {
    AutopilotReader *autopilotReader;
    int targetHeading = 12312312;
    std::thread workerThread;
    std::atomic<bool> running;
    bool settingsValue = false;

    void threadLoop() {
        using namespace std::chrono;
        while (running) {
            const milliseconds loopDelay(300);
            const milliseconds eventRepeatDelay(10);
            std::this_thread::sleep_for(loopDelay);

            updateHeading();
        }
    }

    void updateHeading() {
        using namespace std::chrono;
        const milliseconds eventRepeatDelay(10);
        if (targetHeading == 12312312) {
            return;;
        }

        auto values = autopilotReader->read();

        int heading = values.heading;
        if (heading != targetHeading) {
            int direction = getHeadingRotationDirection(heading, targetHeading);

            int diff = getHeadingDifference(heading, targetHeading);

            SdkWriteConnection::sendInputEvent(connection, inputEvents["AIRLINER_MCU_HDG"], direction);

            for (int i = 2 ; i < diff ; i+=2) {
                std::this_thread::sleep_for(eventRepeatDelay);
                SdkWriteConnection::sendInputEvent(connection, inputEvents["AIRLINER_MCU_HDG"], direction);
            }

        } else {
            settingsValue = false;
        }
    }

    // Returns +1 or -1 for shortest rotation direction on circular heading (0-359)
    int getHeadingRotationDirection(int current, int target) {
        int diff = target - current;
        if (diff > 180) {
            return -1; // Counter-clockwise is shorter
        }
        if (diff < -180) {
            return +1; // Clockwise is shorter
        }
        if (diff > 0) {
            return +1; // Clockwise
        }
        return -1; // Counter-clockwise
    }

    // Returns shortest angular distance between two headings (0-359)
    int getHeadingDifference(int current, int target) {
        int diff = abs(target - current);
        if (diff > 180) {
            return 360 - diff; // Shortest path wraps around
        }
        return diff;
    }

public:
    AirbusWriter(HANDLE *connection,
                 std::unordered_map<std::string, unsigned long long> inputEvents,
                 AutopilotReader *autopilotReader) : MSFSAutopilotWriter(
        connection, inputEvents) {
        this->autopilotReader = autopilotReader;
        this->running = true;
        this->workerThread = std::thread(&AirbusWriter::threadLoop, this);
    }

    ~AirbusWriter() override {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    void setHeading(int headingValue) override {
        targetHeading = headingValue;
        settingsValue = true;
    }

    void toggleSpeed() override {
        SdkWriteConnection::sendInputEvent(connection, inputEvents["AIRLINER_ATHR_PUSH"], 1.0);
    }

    void toggleAutopilot() override {
        SdkWriteConnection::sendInputEvent(connection, inputEvents["AIRLINER_AP1_PUSH"], 1.0);
    }

    bool working() override {
        return settingsValue;
    }
};

#endif //MSFS_CONTROLLER_AIRBUS_WRITER_H
