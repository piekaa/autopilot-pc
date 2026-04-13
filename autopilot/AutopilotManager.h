#ifndef MSFS_CONTROLLER_AUTOPILOTMANAGER_H
#define MSFS_CONTROLLER_AUTOPILOTMANAGER_H

#include <string>

#include "DependencyResolver.h"
#include "rw/MSFSAircraftNameProvider.h"
#include "rw/AutopilotWriter.h"
#include "../serial/Serial.h"
#include "../serial/SerialUpdatesSender.h"

class AutopilotManager {
    SerialUpdatesSender *serialUpdatesSender;
    Serial *serial;

    DependencyResolver *dependencyResolver;

    InputEventsProvider* inputEventsProvider;

    AircractNameProvider *aircraftNameProvider;
    std::string lastAircraftName;

    AutopilotWriter *autopilotWriter = nullptr;
    AutopilotReader *autopilotReader = nullptr;

    bool resolveAircraftIfNew() {
        auto aircraftName = aircraftNameProvider->getAircraftName();
        if (aircraftName != lastAircraftName) {
            lastAircraftName = aircraftName;
            std::cout << "Aircraft name changed to: " << aircraftName << std::endl;
            delete autopilotWriter;
            delete autopilotReader;

            auto inputEvents = inputEventsProvider->getInputEvents();

            for (auto input_event : inputEvents) {
                std::cout << "InputEvent: " << input_event.first << std::endl;
            }

            autopilotReader = dependencyResolver->resolveAutopilotReader(aircraftName);
            autopilotWriter = dependencyResolver->resolveAutopilotWriter(aircraftName, inputEvents, autopilotReader);
            autopilotWriter->setAltitudeIndex();
            serial->setAutopilotWriter(autopilotWriter);

            return true;
        }
        return false;
    }

public:
    AutopilotManager(DependencyResolver *dependencyResolver) {
        this->dependencyResolver = dependencyResolver;
        this->inputEventsProvider = dependencyResolver->resolveInputEventsProvider();
        aircraftNameProvider = dependencyResolver->resolveAircraftNameProvider();
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

            serial->handleCommand();


            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

#endif //MSFS_CONTROLLER_AUTOPILOTMANAGER_H
