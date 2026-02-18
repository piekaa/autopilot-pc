#include <iostream>
#include <thread>
#include <chrono>
#include "SimConnectClient.h"
#include "HeadingReader.h"
#include "SerialPortReader.h"
#include "AutopilotController.h"
#include "CommandProcessor.h"
#include "AutopilotDataReader.h"

/**
 * Main entry point - Java style with clear flow
 * Now with bidirectional serial port control for autopilot!
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

    // Create serial port reader
    SerialPortReader serialPort("COM5");
    if (!serialPort.open(115200)) {
        std::cerr << "Failed to open serial port COM5. Make sure it's available!" << std::endl;
        return 1;
    }

    // Create autopilot controller and initialize
    AutopilotController autopilot(client.getHandle());
    if (!autopilot.initialize()) {
        std::cerr << "Failed to initialize autopilot controller!" << std::endl;
        return 1;
    }

    // Create command processor for incoming commands
    CommandProcessor commandProcessor(serialPort, autopilot);

    // Create autopilot data reader for outgoing data
    AutopilotDataReader dataReader(client.getHandle(), serialPort);
    if (!dataReader.initialize()) {
        std::cerr << "Failed to initialize autopilot data reader!" << std::endl;
        return 1;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "Ready! Bidirectional communication active on COM5" << std::endl;
    std::cout << "Incoming commands from Arduino (single-line format):" << std::endl;
    std::cout << "  H 100 - set heading to 100" << std::endl;
    std::cout << "  S 231 - set speed to 231" << std::endl;
    std::cout << "  A 13000 - set altitude to 13000" << std::endl;
    std::cout << "  VS -1000 - set vertical speed to -1000" << std::endl;
    std::cout << "  X ... - debug log (ignored)" << std::endl;
    std::cout << "Outgoing updates to Arduino (format: H 120, VS 500, A 5000, S 250):" << std::endl;
    std::cout << "  Every 100ms if values changed" << std::endl;
    std::cout << "  Every 5s full update" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::cout << std::endl;

    // Main loop - runs forever until Ctrl+C
    while (true) {
        // Process incoming serial commands
        commandProcessor.processIncomingData();

        // Update autopilot data reader (handles timing and requests)
        dataReader.update();

        // Process SimConnect messages
        DWORD cbData;
        SIMCONNECT_RECV* pData;
        while (SimConnect_GetNextDispatch(client.getHandle(), &pData, &cbData) == S_OK) {
            // Let data reader process its messages
            dataReader.processMessage(pData);
        }

        // Small sleep to avoid burning CPU (like Thread.sleep in Java)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // This won't be reached unless we add a proper exit mechanism
    // But destructor will clean up when program is terminated
    return 0;
}
