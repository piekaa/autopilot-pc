#ifndef MSFS_CONTROLLER_AUTOPILOTVALUES_H
#define MSFS_CONTROLLER_AUTOPILOTVALUES_H

#include <string>
#include <sstream>

struct AutopilotValues {
  double heading;
  double speed;
  double altitude;
  double verticalSpeed;
  double autopilotMaster;
  double headingState;
  double speedState;
  double altitudeState;
  double verticalSpeedState;
  double lNavState;
  double vNavState;

  std::string toCommunicationString() {
    std::ostringstream oss;
    oss << "H " << static_cast<int>(heading) << "\n";
    oss << "S " << static_cast<int>(speed) << "\n";
    oss << "A " << static_cast<int>(altitude) << "\n";
    oss << "VS " << static_cast<int>(verticalSpeed) << "\n";
    oss << "AP " << (autopilotMaster == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_HEADING " << (headingState == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_LNAV " << (lNavState == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_VNAV " << (vNavState == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_SPEED " << (speedState == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_ALTITUDE " << (altitudeState == 0 ? "OFF" : "ON") << "\n";
    oss << "AP_VS " << (verticalSpeedState == 0 ? "OFF" : "ON") << "\n";
    return oss.str();
  }

  bool operator==(const AutopilotValues & autopilot_values) const = default;
};

#endif //MSFS_CONTROLLER_AUTOPILOTVALUES_H