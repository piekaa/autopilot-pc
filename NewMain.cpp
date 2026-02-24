#include "AutopilotReader.h"
#include "GameClient.h"
#include <iostream>


int main() {
    auto gameClient = new GameClient();
    auto autopilotReader = new AutopilotReader(gameClient->getConnection());


    auto data = autopilotReader->read();
    data = autopilotReader->read();
    if (data->heading) {
        std::cout << "Autopilot heading: " << data->heading << std::endl;
    }

    delete autopilotReader;
    delete gameClient;
    return 0;
}
