#ifndef MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H
#define MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H
#include <string>
#include <Windows.h>
#include <SimConnect.h>

class AutopilotWriteField {
    static inline long idGenerator = 0;
    long id;
    std::string eventName;
public:
    int value = 0;

    AutopilotWriteField(std::string eventName) {
        id = idGenerator++;
        this->eventName = eventName;
    }

    long getId() const { return id; }
    std::string getEventName() const { return eventName; }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H