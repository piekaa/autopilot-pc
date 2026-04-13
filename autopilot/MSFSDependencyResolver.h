#ifndef MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H
#define MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H
#include "DependencyResolver.h"
#include "rw/MSFSAutopilotReader.h"
#include "rw/MSFSAutopilotWriter.h"
#include "../aircrafts/AutopilotWriter737.h"
#include "../aircrafts/VisionJetWriter.h"
#include "../aircrafts/AirbusWriter.h"
#include "rw/MSFSInputEventsProvider.h"

class MSFSDependencyResolver : public DependencyResolver {
    HANDLE* nameConnection;
    HANDLE* autopilotConnection;

public:
    MSFSDependencyResolver(HANDLE* nameConnection, HANDLE* autopilotConnection) {
        this->nameConnection = nameConnection;
        this->autopilotConnection = autopilotConnection;
    }


    AutopilotWriter* resolveAutopilotWriter(
        const std::string& aircraftName,
        std::unordered_map<std::string, unsigned long long> inputEvents,
        AutopilotReader* autopilotReader) override {
        if (aircraftName.contains("737") || aircraftName.contains("747")) {
            return new AutopilotWriter737(autopilotConnection, inputEvents);
        }
        if (aircraftName.contains("Vision Jet")) {
            return new VisionJetWriter(autopilotConnection, inputEvents);
        }
        if (aircraftName.contains("A310") || aircraftName.contains("A330")) {
            return new AirbusWriter(autopilotConnection, inputEvents, autopilotReader);
        }

        return new MSFSAutopilotWriter(autopilotConnection, inputEvents);
    }

    AutopilotReader* resolveAutopilotReader(const std::string& aircraftName) override {
        return new MSFSAutopilotReader(autopilotConnection);
    }


    AircractNameProvider* resolveAircraftNameProvider() override {
        return new MSFSAircraftNameProvider(nameConnection);
    }

    InputEventsProvider* resolveInputEventsProvider() override {
        return new MSFSInputEventsProvider(nameConnection);
    }
};

#endif //MSFS_CONTROLLER_MSFSDEPENDENCYRESOLVER_H