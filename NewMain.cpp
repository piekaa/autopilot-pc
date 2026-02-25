#include "AutopilotReader.h"
#include "AutopilotWriter.h"
#include "GameClient.h"
#include <iostream>


int main() {
    auto gameClient = new GameClient();
    auto autopilotReader = new AutopilotReader(gameClient->getConnection());
    auto autopilotWriter = new AutopilotWriter(gameClient->getConnection());

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));


    autopilotWriter->setHeading(321);


    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Autopilot heading: " << autopilotReader->read()->heading << std::endl;
    }


    delete autopilotReader;
    delete gameClient;
    return 0;
}
