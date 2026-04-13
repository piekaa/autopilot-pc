#ifndef MSFS_CONTROLLER_DEPENDENCYRESOLVER_H
#define MSFS_CONTROLLER_DEPENDENCYRESOLVER_H
#include "rw/MSFSAircraftNameProvider.h"
#include "rw/AutopilotReader.h"
#include "rw/AutopilotWriter.h"
#include "rw/InputEventsProvider.h"

class DependencyResolver {
public:
    virtual ~DependencyResolver() = default;

    virtual AutopilotWriter* resolveAutopilotWriter(
        const std::string& aircraftName,
        std::unordered_map<std::string, unsigned long long> inputEvents,
        AutopilotReader* autopilotReader) = 0;
    virtual AutopilotReader* resolveAutopilotReader(const std::string& aircraftName) = 0;
    virtual AircractNameProvider* resolveAircraftNameProvider() = 0;
    virtual InputEventsProvider* resolveInputEventsProvider() = 0;
};

#endif //MSFS_CONTROLLER_DEPENDENCYRESOLVER_H