#include "autopilot/rw/AutopilotReader.h"
#include "autopilot/rw/AutopilotWriter.h"
#include "GameClient.h"
#include <iostream>

#include "autopilot/AutopilotManager.h"


int main() {
    auto autopilotClient = new GameClient();
    auto aircraftNameClient = new GameClient();


    new AutopilotManager(
        aircraftNameClient->getConnection(),
        autopilotClient->getConnection());


    return 0;
}
