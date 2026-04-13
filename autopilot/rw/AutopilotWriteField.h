#ifndef MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H
#define MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H
#include <string>

class AutopilotWriteField {
    static inline long idGenerator = 0;
    long id;
    std::string eventName;
    int value2 = 0;
    bool doubleValued = false;
    std::string units = "";

public:
    int value = 0;

    AutopilotWriteField(std::string eventName, std::string units = "") {
        id = idGenerator++;
        this->eventName = eventName;
        this->units = units;
    }


    void setValue2(int value2) {
        doubleValued = true;
        this->value2 = value2;
    }

    int getValue2() { return value2; }
    bool isDoubleValued() { return doubleValued; }
    long getId() const { return id; }
    std::string getEventName() const { return eventName; }
    std::string getUnits() const { return units; }
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITEFIELD_H
