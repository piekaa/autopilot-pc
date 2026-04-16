#ifndef MSFS_CONTROLLER_AIRBUS320WRITER_H
#define MSFS_CONTROLLER_AIRBUS320WRITER_H
#include "AirbusWriter.h"

class Airbus320Writer : public AirbusWriter {
public:
    Airbus320Writer(HANDLE *connection, const std::unordered_map<std::string, unsigned long long> &inputEvents,
                    AutopilotReader *autopilotReader)
        : AirbusWriter(connection, inputEvents, autopilotReader) {
    }

protected:
    const char *getHeadingEventName() override { return "INSTRUMENT_FCU_HDG_KNOB"; }
    const char *getAltitudeEventName() override { return "INSTRUMENT_FCU_ALT_KNOB"; }
    const char *getSpeedEventName() override { return "INSTRUMENT_FCU_SPD_KNOB"; }
    const char *getVerticalSpeedEventName() override { return "INSTRUMENT_FCU_VS_KNOB"; }
    const char *getAutothrottlePushEventName() override { return "AIRLINER_FCU_SEL_ATHR"; }
    const char *getSpeedPullEventName() override { return "INSTRUMENT_FCU_SPD_PULL"; }
    const char *getHeadingPushEventName() override { return "INSTRUMENT_FCU_HDG_PUSH"; }
    const char *getHeadingPullEventName() override { return "INSTRUMENT_FCU_HDG_PULL"; }
    const char *getAutopilotPushEventName() override { return "AIRLINER_FCU_SEL_AP1"; }
    const char *getVerticalSpeedPushEventName() override { return "INSTRUMENT_FCU_VS_PUSH"; }
    const char *getAltitudePushEventName() override { return "INSTRUMENT_FCU_ALT_PUSH"; }
};

#endif //MSFS_CONTROLLER_AIRBUS320WRITER_H
