#include "AutopilotReader.h"
#include "AutopilotWriter.h"
#include "GameClient.h"
#include <iostream>

#include "AutopilotManager.h"


int main() {
    auto autopilotClient = new GameClient();
    auto aircraftNameClient = new GameClient();


    auto manager = new AutopilotManager(aircraftNameClient->getConnection());

    auto autopilotReader = new AutopilotReader(autopilotClient->getConnection());
    auto autopilotWriter = new AutopilotWriter(autopilotClient->getConnection());

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));


    autopilotWriter->setHeading(321);


    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Autopilot heading: " << autopilotReader->read()->heading << std::endl;
    }


    delete autopilotReader;
    delete autopilotClient;
    delete aircraftNameClient;
    return 0;
}
