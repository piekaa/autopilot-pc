#ifndef MSFS_CONTROLLER_FAKEINPUTEVENTSPROVIDER_H
#define MSFS_CONTROLLER_FAKEINPUTEVENTSPROVIDER_H
#include "InputEventsProvider.h"

class FakeInputEventsProvider : public InputEventsProvider{
    std::unordered_map<std::string, unsigned long long> getInputEvents() override {
        std::unordered_map<std::string, unsigned long long> map;
        return map;
    }
};

#endif //MSFS_CONTROLLER_FAKEINPUTEVENTSPROVIDER_H