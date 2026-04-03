#ifndef MSFS_CONTROLLER_AUTOPILOTWRITER_H
#define MSFS_CONTROLLER_AUTOPILOTWRITER_H

class AutopilotWriter {
public:
    virtual ~AutopilotWriter() = default;

    virtual void setHeading(int headingValue) = 0;
    virtual void setAltitude(int altitudeValue) = 0;
    virtual void setSpeed(int speedValue) = 0;
    virtual void setVerticalSpeed(int verticalSpeedValue) = 0;
    virtual void toggleAutopilot() = 0;
    virtual void toggleHeading() = 0;
    virtual void toggleSpeed() = 0;
    virtual void toggleAltitude() = 0;
    virtual void toggleVerticalSpeed() = 0;
    virtual void toggleLNav() = 0;
    virtual void toggleVNav() = 0;
    virtual void setAltitudeIndex() = 0;
};

#endif //MSFS_CONTROLLER_AUTOPILOTWRITER_H
