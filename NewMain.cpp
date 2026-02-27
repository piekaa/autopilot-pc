#include "AutopilotReader.h"
#include "AutopilotWriter.h"
#include "GameClient.h"
#include <iostream>

#include "AutopilotManager.h"


int main() {
    auto autopilotClient = new GameClient();
    auto aircraftNameClient = new GameClient();


    new AutopilotManager(
        aircraftNameClient->getConnection(),
        autopilotClient->getConnection());


    return 0;
}
