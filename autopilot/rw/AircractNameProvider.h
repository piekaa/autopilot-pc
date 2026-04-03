
#ifndef MSFS_CONTROLLER_AIRCRACTNAMEPROVIDER_H
#define MSFS_CONTROLLER_AIRCRACTNAMEPROVIDER_H
#include <string>

class AircractNameProvider {
public:
    virtual ~AircractNameProvider() = default;

    virtual std::string getAircraftName() = 0;
};

#endif //MSFS_CONTROLLER_AIRCRACTNAMEPROVIDER_H