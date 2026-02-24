#include "AutopilotReader.h"
#include "GameClient.h"
#include <iostream>


int main() {
    auto gameClient = new GameClient();
    auto autopilotReader = new AutopilotReader(gameClient->getConnection());

    std::cout << "Autopilot heading: " << autopilotReader->read()->heading << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "Autopilot heading: " << autopilotReader->read()->heading << std::endl;


    delete autopilotReader;
    delete gameClient;
    return 0;
}
