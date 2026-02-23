#ifndef MSFS_CONTROLLER_SIMCONNECTREADONLYFIELD_H
#define MSFS_CONTROLLER_SIMCONNECTREADONLYFIELD_H
#include <string>

class SimConnectReadonlyField {
    std::string fieldName;
    std::string units;
    long simConnectEventId;
    double value;
public:
    explicit SimConnectReadonlyField(std::string fieldName, std::string units) {
        this->fieldName = fieldName;
        this->units = units;
    }

    double getValue() { return value; }
    std::string getFieldName() const { return fieldName; }
    std::string getUnits() const { return units; }
};

#endif //MSFS_CONTROLLER_SIMCONNECTREADONLYFIELD_H