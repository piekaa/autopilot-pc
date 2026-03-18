#ifndef MSFS_CONTROLLER_AUTOPILOTVALUES_H
#define MSFS_CONTROLLER_AUTOPILOTVALUES_H

#include <string>
#include <sstream>

struct AutopilotValues {
  double heading;
  double speed;
  double altitude;
  double verticalSpeed;

  std::string toCommunicationString() {
    std::ostringstream oss;
    oss << "H " << static_cast<int>(heading) << "\n";
    oss << "S " << static_cast<int>(speed) << "\n";
    oss << "A " << static_cast<int>(altitude) << "\n";
    oss << "VS " << static_cast<int>(verticalSpeed) << "\n";
    return oss.str();
  }

  bool operator==(const AutopilotValues & autopilot_values) const = default;
};

#endif //MSFS_CONTROLLER_AUTOPILOTVALUES_H