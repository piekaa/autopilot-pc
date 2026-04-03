#ifndef MSFS_CONTROLLER_AUTOPILOTREADER_H
#define MSFS_CONTROLLER_AUTOPILOTREADER_H

#include "AutopilotValues.h"

class AutopilotReader {
public:
    virtual ~AutopilotReader() = default;

    virtual AutopilotValues read() = 0;
};


#endif //MSFS_CONTROLLER_AUTOPILOTREADER_H
