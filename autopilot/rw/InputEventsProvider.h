#ifndef MSFS_CONTROLLER_INPUTEVENTSPROVIDER_H
#define MSFS_CONTROLLER_INPUTEVENTSPROVIDER_H
#include <string>
#include <unordered_map>

class InputEventsProvider {
public:
    virtual ~InputEventsProvider() = default;

    virtual std::unordered_map<std::string, unsigned long long>  getInputEvents() = 0;
};

#endif //MSFS_CONTROLLER_INPUTEVENTSPROVIDER_H