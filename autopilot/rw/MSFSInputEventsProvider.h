#ifndef MSFS_CONTROLLER_MSFSINPUTEVENTSPROVIDER_H
#define MSFS_CONTROLLER_MSFSINPUTEVENTSPROVIDER_H
#include <string>
#include <unordered_map>

#include "MSFSAircraftNameProvider.h"
#include "InputEventsProvider.h"

class MSFSInputEventsProvider : public InputEventsProvider {
    HANDLE *connection;

public:
    MSFSInputEventsProvider(HANDLE *connection) {
        this->connection = connection;
    }

    std::unordered_map<std::string, unsigned long long> getInputEvents() override {
        SdkReadConnection::requestEnumerateInputEvents(connection);
        return SdkReadConnection::readEnumerations(connection);
    }
};

#endif //MSFS_CONTROLLER_MSFSINPUTEVENTSPROVIDER_H
