#ifndef MSFS_CONTROLLER_AUTOPILOTVALUES_H
#define MSFS_CONTROLLER_AUTOPILOTVALUES_H

#include <string>
#include <sstream>

struct AutopilotValues {
  double heading;
  double altitude;

  std::string toCommunicationString() {
    std::ostringstream oss;
    oss << "H " << static_cast<int>(heading) << "\n";
    oss << "A " << static_cast<int>(altitude) << "\n";
    return oss.str();
  }
};

#endif //MSFS_CONTROLLER_AUTOPILOTVALUES_H