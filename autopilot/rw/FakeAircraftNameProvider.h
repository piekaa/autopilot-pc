#ifndef MSFS_CONTROLLER_FAKEAIRCRAFTNAMEPROVIDER_H
#define MSFS_CONTROLLER_FAKEAIRCRAFTNAMEPROVIDER_H
#include "AircractNameProvider.h"

class FakeAircraftNameProvider : public AircractNameProvider {

    std::string getAircraftName() override {
        return "FakeAircraft";
    }
};

#endif //MSFS_CONTROLLER_FAKEAIRCRAFTNAMEPROVIDER_H