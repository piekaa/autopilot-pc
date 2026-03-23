#ifndef MSFS_CONTROLLER_AUTOPILOTREADER_H
#define MSFS_CONTROLLER_AUTOPILOTREADER_H

#include <Windows.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "AutopilotValues.h"
#include "../sdk/SdkReadConnection.h"

class AutopilotReader {
    HANDLE* connection;
    std::thread workerThread;
    std::atomic<bool> running;
    std::mutex dataMutex;
    AutopilotValues* lastValues;

    void threadLoop() {
        using namespace std::chrono;
        const milliseconds requestInterval(200);
        auto lastRequestTime = steady_clock::now() - requestInterval;
        const milliseconds readInterval(50);

        while (running) {
            auto currentTime = steady_clock::now();

            if (duration_cast<milliseconds>(currentTime - lastRequestTime) >= requestInterval) {
                SdkReadConnection::requestAutopilotData(connection);
                lastRequestTime = currentTime;
            }

            {
                std::lock_guard lock(dataMutex);
                auto data = SdkReadConnection::readAutopilotData(connection);
                if (data) {
                    delete lastValues;
                    lastValues =  data;
                }
            }
            std::this_thread::sleep_for(readInterval);
        }
    }

public:
    AutopilotReader(HANDLE* connection) {
        this->connection = connection;
        this->running = false;
        this->lastValues = new AutopilotValues();
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT HEADING LOCK DIR", "degrees");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT AIRSPEED HOLD VAR", "knots");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT ALTITUDE LOCK VAR", "feet");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT VERTICAL HOLD VAR", "feet per minute");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT MASTER", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT HEADING LOCK", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT AIRSPEED HOLD", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT ALTITUDE LOCK", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT VERTICAL HOLD", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT NAV1 LOCK", "Bool");
        SdkReadConnection::registerAutopilotField(connection, "AUTOPILOT FLIGHT LEVEL CHANGE", "Bool");


        // double vNavState;
        this->running = true;
        this->workerThread = std::thread(&AutopilotReader::threadLoop, this);
    }

    ~AutopilotReader() {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
        delete lastValues;
    }

    AutopilotValues read() {
        std::lock_guard lock(dataMutex);
        return AutopilotValues(*lastValues);
    }
};


#endif //MSFS_CONTROLLER_AUTOPILOTREADER_H