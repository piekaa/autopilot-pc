#ifndef MSFS_CONTROLLER_FAKEDEPENDENCYRESOLVER_H
#define MSFS_CONTROLLER_FAKEDEPENDENCYRESOLVER_H
#include "DependencyResolver.h"
#include "rw/FakeAutopilotReader.h"
#include "rw/FakeAutopilotWriter.h"
#include "rw/FakeAircraftNameProvider.h"
#include "rw/FakeInputEventsProvider.h"

class FakeDependencyResolver : public DependencyResolver {
public:
    AutopilotWriter* resolveAutopilotWriter(
        const std::string& aircraftName,
        std::unordered_map<std::string, unsigned long long> inputEvents,
        AutopilotReader* autopilotReader) override {
        return new FakeAutopilotWriter();
    }

    AutopilotReader* resolveAutopilotReader(const std::string& aircraftName) override {
        return new FakeAutopilotReader();
    }

    AircractNameProvider* resolveAircraftNameProvider() override {
        return new FakeAircraftNameProvider();
    }

    InputEventsProvider* resolveInputEventsProvider() override {
        return new FakeInputEventsProvider();
    }
};

#endif //MSFS_CONTROLLER_FAKEDEPENDENCYRESOLVER_H