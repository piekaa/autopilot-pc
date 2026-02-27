#ifndef MSFS_CONTROLLER_AUTOPILORWRITER737_H
#define MSFS_CONTROLLER_AUTOPILORWRITER737_H

#include <windows.h>
#include "../AutopilotWriter.h"

class AutopilotWriter737 : public AutopilotWriter{

public:

    AutopilotWriter737(HANDLE* connection) : AutopilotWriter(connection) {
        altitudeIndex->value = 3;
    }

    void setAltitude(int altitudeValue) override {
        altitude->setValue2(3);
        AutopilotWriter::setAltitude(altitudeValue);
    }

};

#endif //MSFS_CONTROLLER_AUTOPILORWRITER737_H