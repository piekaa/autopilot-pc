#ifndef MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H
#define MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H
#include "DependencyResolver.h"
#include "rw/AutopilotReader.h"
#include "rw/AutopilotWriter.h"
#include "../aircrafts/AutopilotWriter747.h"
#include "rw/MSFSInputEventsProvider.h"

class MSFSDependencyResolver : public DependencyResolver {
    HANDLE* nameConnection;
    HANDLE* autopilotConnection;

public:
    MSFSDependencyResolver(HANDLE* nameConnection, HANDLE* autopilotConnection) {
        this->nameConnection = nameConnection;
        this->autopilotConnection = autopilotConnection;
    }


    AutopilotWriter* resolveAutopilotWriter(const std::string& aircraftName, std::unordered_map<std::string, unsigned long long> inputEvents) override {
        if (aircraftName.contains("737")) {
            return new AutopilotWriter737(autopilotConnection, inputEvents);
        }
        return new AutopilotWriter(autopilotConnection, inputEvents);
    }

    AutopilotReader* resolveAutopilotReader(const std::string& aircraftName) override {
        return new AutopilotReader(autopilotConnection);
    }


    AircractNameProvider* resolveAircraftNameProvider() override {
        return new MSFSAircraftNameProvider(nameConnection);
    }

    InputEventsProvider* resolveInputEventsProvider() override {
        return new MSFSInputEventsProvider(nameConnection);
    }
};

#endif //MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H