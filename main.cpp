#include <iostream>
#include <thread>
#include <chrono>
#include "SimConnectClient.h"
#include "HeadingReader.h"
#include "SerialPortReader.h"
#include "AutopilotController.h"
#include "CommandProcessor.h"

/**
 * Main entry point - Java style with clear flow
 * Now with serial port control for autopilot!
 */
int main() {
    std::cout << "=== MSFS SimConnect Autopilot Controller ===" << std::endl;
    std::cout << "Connecting to Microsoft Flight Simulator 2024..." << std::endl;

    // Create SimConnect client (like instantiating a Java object)
    SimConnectClient client;

    // Try to connect
    if (!client.connect("MSFS Autopilot Controller")) {
        std::cerr << "Failed to connect. Make sure MSFS 2024 is running!" << std::endl;
        return 1;
    }

    // Create autopilot controller and initialize
    AutopilotController autopilot(client.getHandle());
    if (!autopilot.initialize()) {
        std::cerr << "Failed to initialize autopilot controller!" << std::endl;
        return 1;
    }

    // Create serial port reader
    SerialPortReader serialPort("COM5");
    if (!serialPort.open(115200)) {
        std::cerr << "Failed to open serial port COM5. Make sure it's available!" << std::endl;
        return 1;
    }

    // Create command processor
    CommandProcessor commandProcessor(serialPort, autopilot);

    std::cout << "========================================" << std::endl;
    std::cout << "Ready! Listening for commands on COM5" << std::endl;
    std::cout << "Command format:" << std::endl;
    std::cout << "  Line 1: Command type (H, VS, A, S)" << std::endl;
    std::cout << "  Line 2: Value (+, -)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::cout << std::endl;

    // Main loop - runs forever until Ctrl+C
    while (true) {
        // Process incoming serial commands
        commandProcessor.processIncomingData();

        // Process SimConnect messages
        DWORD cbData;
        SIMCONNECT_RECV* pData;
        while (SimConnect_GetNextDispatch(client.getHandle(), &pData, &cbData) == S_OK) {
            // Process any SimConnect messages if needed
        }

        // Small sleep to avoid burning CPU (like Thread.sleep in Java)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // This won't be reached unless we add a proper exit mechanism
    // But destructor will clean up when program is terminated
    return 0;
}
