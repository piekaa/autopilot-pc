#include <iostream>
#include <thread>
#include <chrono>
#include "SimConnectClient.h"
#include "HeadingReader.h"

/**
 * Main entry point - Java style with clear flow
 */
int main() {
    std::cout << "=== MSFS SimConnect Hello World ===" << std::endl;
    std::cout << "Connecting to Microsoft Flight Simulator 2024..." << std::endl;

    // Create SimConnect client (like instantiating a Java object)
    SimConnectClient client;

    // Try to connect
    if (!client.connect("MSFS Hello World")) {
        std::cerr << "Failed to connect. Make sure MSFS 2024 is running!" << std::endl;
        return 1;
    }

    // Create heading reader and initialize
    HeadingReader headingReader(client.getHandle());
    if (!headingReader.initialize()) {
        std::cerr << "Failed to initialize heading reader!" << std::endl;
        return 1;
    }

    std::cout << "Running for 5 seconds..." << std::endl;
    std::cout << "Reading autopilot heading every second:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Run for 5 seconds, requesting heading every second
    auto startTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::seconds(5);

    while (std::chrono::steady_clock::now() - startTime < duration) {
        // Request heading data
        headingReader.requestHeading();

        // Process messages for 1 second
        auto loopStart = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - loopStart < std::chrono::milliseconds(1000)) {
            // Process SimConnect messages
            DWORD cbData;
            SIMCONNECT_RECV* pData;

            while (SimConnect_GetNextDispatch(client.getHandle(), &pData, &cbData) == S_OK) {
                headingReader.processMessage(pData);
            }

            // Small sleep to avoid burning CPU (like Thread.sleep in Java)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "5 seconds elapsed. Disconnecting..." << std::endl;

    // Client will auto-disconnect in destructor (like Java's try-with-resources)
    return 0;
}
