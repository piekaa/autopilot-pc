# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

MSFS-controller is a C++ application that provides bidirectional serial communication between an Arduino and Microsoft Flight Simulator 2024 via the SimConnect SDK. It allows external hardware to control autopilot functions and receive real-time flight data.

MSFS 2024 SDK docs are here https://docs.flightsimulator.com/msfs2024/html/1_Introduction/SDK_Overview.htm



## Build System

This project uses CMake with C++23. The SimConnect SDK is required and must be installed at:
`C:/MSFS 2024 SDK/SimConnect SDK`

### Build Commands

```bash
# Configure and build using CMake
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release

# Or using Ninja (if available)
cmake -B cmake-build-debug -G Ninja
cmake --build cmake-build-debug

# Run the executable
./cmake-build-debug/MSFS_controller.exe
```

The build automatically copies `SimConnect.dll` to the output directory.

## Architecture

The application follows a modular, event-driven architecture with these core components:

### Core Components

1. **SimConnectClient** - Wrapper for SimConnect SDK connection lifecycle
   - Manages connection/disconnection to MSFS 2024
   - Provides handle for other components to interact with SimConnect

2. **SerialPortReader** - Windows serial port communication
   - Opens COM5 at 115200 baud (configurable in main.cpp:29)
   - Provides both reading and writing capabilities via Windows API
   - Non-blocking data availability checks

3. **AutopilotController** - Sends commands from Arduino to MSFS
   - Maps SimConnect events for heading, altitude, vertical speed, and airspeed
   - Translates serial commands into SimConnect event transmissions
   - Uses absolute value setting (not increment/decrement)

4. **CommandProcessor** - Parses incoming serial commands
   - Single-line command format: `<CMD> <VALUE>`
   - Supported commands: `H` (heading), `S` (speed), `A` (altitude), `VS` (vertical speed)
   - Debug logs prefixed with `X` are ignored

5. **AutopilotDataReader** - Reads autopilot state from MSFS and sends to Arduino
   - Polls SimConnect for autopilot heading, altitude, vertical speed, and airspeed
   - Change detection: only sends updates when values change beyond threshold (0.5 units default)
   - Periodic full updates every 5 seconds
   - Fast updates every 100ms on value changes

### Data Flow

**Incoming (Arduino → MSFS):**
Serial COM5 → SerialPortReader → CommandProcessor → AutopilotController → SimConnect → MSFS

**Outgoing (MSFS → Arduino):**
MSFS → SimConnect → AutopilotDataReader (polling) → SerialPortReader → Serial COM5

### Main Loop (main.cpp:68-85)

The application runs a continuous loop that:
1. Processes incoming serial commands
2. Updates autopilot data reader (handles timing/requests)
3. Processes SimConnect message dispatch queue
4. Sleeps 10ms to prevent CPU saturation

## SimConnect Integration

The project uses SimConnect SDK's C API. Key patterns:

- **Event mapping**: Events must be mapped with `SimConnect_MapClientEventToSimEvent` before transmission
- **Data definitions**: Data structures must be defined with `SimConnect_AddToDataDefinition` before requesting
- **Message dispatch**: `SimConnect_GetNextDispatch` must be called regularly to receive data responses
- **Handle sharing**: The SimConnect handle from SimConnectClient is shared across AutopilotController and AutopilotDataReader

## Serial Protocol

All commands are one line of text, first word is command name. Debug logs always start with X.

**Incoming commands (Arduino → PC):**
- Set heading: `H <value>` - Example: `H 100` sets autopilot heading to 100°
- Set speed: `S <value>` - Example: `S 231` sets autopilot speed to 231 knots
- Set altitude: `A <value>` - Example: `A 13000` sets autopilot altitude to 13000 feet
- Set vertical speed: `VS <value>` - Example: `VS -1000` sets vertical speed to -1000 fpm
- Toggle autopilot: `AP TOGGLE` - Turns autopilot master on/off
- Debug logs: `X <message>` - Example: `X sensor initiated` (ignored by processor)

**Outgoing data (PC → Arduino):**
- Heading update: `H <value>` - Example: `H 100` informs heading is at 100°
- Speed update: `S <value>` - Example: `S 231` informs speed is at 231 knots
- Altitude update: `A <value>` - Example: `A 13000` informs altitude is at 13000 feet
- Vertical speed update: `VS <value>` - Example: `VS -1000` informs VS is at -1000 fpm
- Autopilot status: `AP ON` or `AP OFF` - Informs current autopilot master state
- Updates sent on change (>0.5 unit threshold for H/S, >10 units for A/VS) or every 5 seconds

## Important Notes

- The application requires MSFS 2024 to be running before connecting
- Serial port is hardcoded to COM5 - change in main.cpp:29 if needed
- SimConnect SDK path is hardcoded in CMakeLists.txt:7
- No graceful shutdown mechanism - terminated via Ctrl+C
- All SimConnect event transmissions use `SIMCONNECT_GROUP_PRIORITY_HIGHEST`
- The codebase style follows "Java-style" conventions with explicit comments
