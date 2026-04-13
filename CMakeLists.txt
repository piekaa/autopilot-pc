cmake_minimum_required(VERSION 3.22)
project(MSFS_controller)

set(CMAKE_CXX_STANDARD 23)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# SimConnect SDK paths
set(SIMCONNECT_SDK_PATH "C:/MSFS 2024 SDK/SimConnect SDK")
set(SIMCONNECT_INCLUDE_DIR "${SIMCONNECT_SDK_PATH}/include")
set(SIMCONNECT_LIB_DIR "${SIMCONNECT_SDK_PATH}/lib")

# Include directories
include_directories(${SIMCONNECT_INCLUDE_DIR})

# Add all source files
add_executable(MSFS_controller
        SimConnectClient.cpp
    SimConnectClient.h
    HeadingReader.cpp
    HeadingReader.h
        AutopilotController.cpp
    AutopilotController.h
        serial/CommandProcessor.h
        NewMain.cpp GameClient.h
        autopilot/rw/AutopilotReader.h
        autopilot/sdk/SdkReadConnection.h
        autopilot/rw/AutopilotValues.h
        autopilot/rw/AutopilotWriter.h
        autopilot/rw/AutopilotWriteField.h
        autopilot/sdk/SdkWriteConnection.h
        autopilot/AutopilotManager.h
        autopilot/rw/MSFSAircraftNameProvider.h
        aircrafts/AutopilotWriter737.h
        serial/SerialUpdatesSender.h
        serial/SerialCommandDiff.h
        autopilot/MSFSDependencyResolver.h
        autopilot/DependencyResolver.h
        autopilot/rw/InputEventsProvider.h
        autopilot/FakeDependencyResolver.h
        autopilot/rw/FakeInputEventsProvider.h
        autopilot/rw/AircractNameProvider.h
        autopilot/rw/FakeAircraftNameProvider.h
        autopilot/rw/FakeAutopilotWriter.h
        aircrafts/VisionJetWriter.h
        autopilot/sdk/DoubleValue.h
)

# Link against SimConnect and Windows libraries
target_link_libraries(MSFS_controller
    ${SIMCONNECT_LIB_DIR}/SimConnect.lib
    shlwapi
    user32
    Ws2_32
)

# Copy SimConnect.dll to output directory after build (so it can run)
add_custom_command(TARGET MSFS_controller POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SIMCONNECT_LIB_DIR}/SimConnect.dll"
    $<TARGET_FILE_DIR:MSFS_controller>
)

# Tests
add_executable(SerialCommandDiffTest
    tests/SerialCommandDiffTest.cpp
    serial/SerialCommandDiff.h
)
target_link_libraries(SerialCommandDiffTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(SerialCommandDiffTest)
