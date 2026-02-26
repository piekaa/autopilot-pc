#ifndef MSFS_CONTROLLER_AUTOPILOTMANAGER_H
#define MSFS_CONTROLLER_AUTOPILOTMANAGER_H

#include <windows.h>
#include "AircraftNameProvider.h"

class AutopilotManager {
    HANDLE* connection;
    AircraftNameProvider* aircraftNameProvider;

public:
    AutopilotManager(HANDLE* connection) {
        this->connection = connection;
        aircraftNameProvider = new AircraftNameProvider(connection);

        std::this_thread::sleep_for(std::chrono::milliseconds(4000));

        std::cout << "Aircraft name: " << aircraftNameProvider->getAircraftName() << std::endl;
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTMANAGER_H