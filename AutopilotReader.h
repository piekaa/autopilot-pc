#ifndef MSFS_CONTROLLER_AUTOPILOTREADER_H
#define MSFS_CONTROLLER_AUTOPILOTREADER_H

#include <Windows.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "AutopilotValues.h"
#include "SdkReadConnection.h"

class AutopilotReader {
    HANDLE* connection;
    std::thread workerThread;
    std::atomic<bool> running;
    std::mutex dataMutex;
    AutopilotValues* lastValues;

    void threadLoop() {
        using namespace std::chrono;
        auto lastRequestTime = steady_clock::now();
        const milliseconds requestInterval(200);
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

    AutopilotValues* read() {
        std::lock_guard lock(dataMutex);
        return lastValues;
    }
};


#endif //MSFS_CONTROLLER_AUTOPILOTREADER_H