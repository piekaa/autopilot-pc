#ifndef MSFS_CONTROLLER_GAMECLIENT_H
#define MSFS_CONTROLLER_GAMECLIENT_H

#include <iostream>
#include <Windows.h>
#include <SimConnect.h>
#include <string>

class GameClient {
    HANDLE* connection = new HANDLE();
public:
    GameClient() {
        // Create connection name with a simple ID
        std::string connectionName = "Piekoszek: " + std::to_string(GetTickCount64());

        HRESULT hr = SimConnect_Open(
            connection,
            connectionName.c_str(),
            nullptr,
               0,
               nullptr,
               0
           );
        if (hr == S_OK) {
            std::cout << "Successfully connected to MSFS!" << std::endl;
        } else {
            std::cerr << "Failed to connect to MSFS." << std::endl;
        }
    }

    ~GameClient() {
        HRESULT hr = SimConnect_Close(*connection);
        if (hr == S_OK) {
            std::cout << "Disconnected" << std::endl;
        } else {
            std::cerr << "Failed to disconnect from MSFS." << std::endl;
        }
    }

    HANDLE* getConnection() const {
        return connection;
    }
};


#endif //MSFS_CONTROLLER_GAMECLIENT_H
