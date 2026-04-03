#ifndef MSFS_CONTROLLER_FAKEAUTOPILOTREADER_H
#define MSFS_CONTROLLER_FAKEAUTOPILOTREADER_H

#include "AutopilotReader.h"

class FakeAutopilotReader : public AutopilotReader {
public:
    AutopilotValues read() override {
        AutopilotValues values;
        values.heading = 100.0;
        values.speed = 250.0;
        values.altitude = 10000.0;
        values.verticalSpeed = 0.0;
        values.autopilotMaster = 1.0;
        values.headingState = 1.0;
        values.speedState = 1.0;
        values.altitudeState = 1.0;
        values.verticalSpeedState = 0.0;
        values.lNavState = 0.0;
        values.vNavState = 0.0;
        return values;
    }
};

#endif //MSFS_CONTROLLER_FAKEAUTOPILOTREADER_H
