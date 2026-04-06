#include "GameClient.h"

#include "autopilot/AutopilotManager.h"
#include "autopilot/FakeDependencyResolver.h"
#include "autopilot/MSFSDependencyResolver.h"


int main() {


    DependencyResolver* dependencyResolver = new MSFSDependencyResolver(
    (new GameClient())->getConnection(),
    (new GameClient())->getConnection());

    // DependencyResolver* dependencyResolver = new FakeDependencyResolver();

    new AutopilotManager(dependencyResolver);


    return 0;
}
