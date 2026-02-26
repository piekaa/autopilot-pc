#ifndef MSFS_CONTROLLER_AIRCRAFTNAMEPROVIDER_H
#define MSFS_CONTROLLER_AIRCRAFTNAMEPROVIDER_H

#include <windows.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cstring>
#include <string>

#include "SdkReadConnection.h"

class AircraftNameProvider {
    HANDLE* connection;
    std::thread workerThread;
    std::atomic<bool> running;
    std::mutex dataMutex;
    char currentAircraftName[256];

    void threadLoop() {
        using namespace std::chrono;
        const milliseconds requestInterval(5000);  // Request every 5 seconds
        auto lastRequestTime = steady_clock::now() - requestInterval;
        const milliseconds readInterval(500);       // Check for data every 500ms

        while (running) {
            auto currentTime = steady_clock::now();

            if (duration_cast<milliseconds>(currentTime - lastRequestTime) >= requestInterval) {
                SdkReadConnection::requestAircraftName(connection);
                lastRequestTime = currentTime;
            }

            {
                std::lock_guard lock(dataMutex);
                auto data = SdkReadConnection::readAircraftName(connection);
                if (data) {
                    strncpy_s(currentAircraftName, 256, data, 255);
                    currentAircraftName[255] = '\0';  // Ensure null termination
                }
            }
            std::this_thread::sleep_for(readInterval);
        }
    }

public:
    AircraftNameProvider(HANDLE* connection) {
        this->connection = connection;
        this->running = false;
        memset(currentAircraftName, 0, 256);

        SdkReadConnection::registerForAircraftName(connection);

        this->running = true;
        this->workerThread = std::thread(&AircraftNameProvider::threadLoop, this);
    }

    ~AircraftNameProvider() {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    std::string getAircraftName() {
        std::lock_guard lock(dataMutex);
        return std::string(currentAircraftName);
    }
};

#endif //MSFS_CONTROLLER_AIRCRAFTNAMEPROVIDER_H