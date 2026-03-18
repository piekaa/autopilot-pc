#ifndef MSFS_CONTROLLER_AUTOPILOTMANAGER_H
#define MSFS_CONTROLLER_AUTOPILOTMANAGER_H

#include <windows.h>
#include <string>
#include "../AircraftNameProvider.h"
#include "rw/AutopilotWriter.h"
#include "../serial/Serial.h"
#include "../serial/SerialUpdatesSender.h"
#include "../aircrafts/AutopilotWriter747.h"

class AutopilotManager {
    SerialUpdatesSender* serialUpdatesSender;
    Serial* serial;
    HANDLE* nameConnection;
    HANDLE* autopilotConnection;
    AircraftNameProvider* aircraftNameProvider;
    std::string lastAircraftName;
    
    AutopilotWriter* autopilotWriter = nullptr;
    AutopilotReader* autopilotReader = nullptr;

    bool resolveAircraftIfNew() {
        auto aircraftName = aircraftNameProvider->getAircraftName();
        if (aircraftName != lastAircraftName) {
            lastAircraftName = aircraftName;
            std::cout << "Aircraft name changed to: " << aircraftName << std::endl;
            delete autopilotWriter;
            delete autopilotReader;

            SdkReadConnection::requestEnumerateInputEvents(autopilotConnection);
            auto inputEvents = SdkReadConnection::readEnumerations(autopilotConnection);

            autopilotWriter = resolveAutopilotWriter(aircraftName, inputEvents);
            autopilotWriter->setAltitudeIndex();
            autopilotReader = resolveAutopilotReader(aircraftName);
            return true;
        }
        return false;
    }

    AutopilotWriter* resolveAutopilotWriter(const std::string& aircraftName, std::unordered_map<std::string, unsigned long long> inputEvents) {
        if (aircraftName.contains("737")) {
            return new AutopilotWriter737(autopilotConnection, inputEvents);
        }
        return new AutopilotWriter(autopilotConnection, inputEvents);
    }

    AutopilotReader* resolveAutopilotReader(const std::string& aircraftName) {
        return new AutopilotReader(autopilotConnection);
    }
    
public:
    AutopilotManager(HANDLE* connection, HANDLE* autopilotConnection) {
        this->nameConnection = connection;
        this->autopilotConnection = autopilotConnection;
        aircraftNameProvider = new AircraftNameProvider(connection);
        serial = new Serial();
        serialUpdatesSender = new SerialUpdatesSender(serial);

        for (;;) {

            if (resolveAircraftIfNew()) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        for (;;) {
            resolveAircraftIfNew();

            auto values = autopilotReader->read();
            serialUpdatesSender->sendIfNeeded(values);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
};

#endif //MSFS_CONTROLLER_AUTOPILOTMANAGER_H