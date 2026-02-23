#include "AutopilotReader.h"
#include "GameClient.h"

int main() {
    auto gameClient = new GameClient();
    auto autopilotReader = new AutopilotReader(gameClient->getConnection());




    delete gameClient;
    return 0;
}
