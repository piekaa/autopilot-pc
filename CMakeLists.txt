cmake_minimum_required(VERSION 3.22)
project(MSFS_controller)

set(CMAKE_CXX_STANDARD 23)

# SimConnect SDK paths
set(SIMCONNECT_SDK_PATH "C:/MSFS 2024 SDK/SimConnect SDK")
set(SIMCONNECT_INCLUDE_DIR "${SIMCONNECT_SDK_PATH}/include")
set(SIMCONNECT_LIB_DIR "${SIMCONNECT_SDK_PATH}/lib")

# Include directories
include_directories(${SIMCONNECT_INCLUDE_DIR})

# Add all source files
add_executable(MSFS_controller
    main.cpp
    SimConnectClient.cpp
    SimConnectClient.h
    HeadingReader.cpp
    HeadingReader.h
    SerialPortReader.cpp
    SerialPortReader.h
    AutopilotController.cpp
    AutopilotController.h
    CommandProcessor.cpp
    CommandProcessor.h
    AutopilotDataReader.cpp
    AutopilotDataReader.h
        NewMain.cpp GameClient.h
        AutopilotReader.h
        SdkConnection.h
        AutopilotValues.h)

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
