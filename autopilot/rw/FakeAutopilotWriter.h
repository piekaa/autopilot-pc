#ifndef MSFS_CONTROLLER_FAKEAUTOPILOTWRITER_H
#define MSFS_CONTROLLER_FAKEAUTOPILOTWRITER_H

#include "AutopilotWriter.h"

class FakeAutopilotWriter : public AutopilotWriter {
public:
    void setHeading(int headingValue) override {
        // Fake implementation - no-op
    }

    void setAltitude(int altitudeValue) override {
        // Fake implementation - no-op
    }

    void setSpeed(int speedValue) override {
        // Fake implementation - no-op
    }

    void setVerticalSpeed(int verticalSpeedValue) override {
        // Fake implementation - no-op
    }

    void toggleAutopilot() override {
        // Fake implementation - no-op
    }

    void toggleHeading() override {
        // Fake implementation - no-op
    }

    void toggleSpeed() override {
        // Fake implementation - no-op
    }

    void toggleAltitude() override {
        // Fake implementation - no-op
    }

    void toggleVerticalSpeed() override {
        // Fake implementation - no-op
    }

    void toggleLNav() override {
        // Fake implementation - no-op
    }

    void toggleVNav() override {
        // Fake implementation - no-op
    }

    void setAltitudeIndex()  override {
        // Fake implementation - no-op
    };

    bool working() override {
        return false;
    }
};

#endif //MSFS_CONTROLLER_FAKEAUTOPILOTWRITER_H
