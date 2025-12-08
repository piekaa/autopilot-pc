#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <Windows.h>
#include <string>
#include <functional>

/**
 * SerialPortReader class (Java-style)
 * Handles reading data from a serial port (like a Java InputStream)
 */
class SerialPortReader {
private:
    HANDLE hSerial;
    std::string portName;
    bool connected;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

public:
    // Constructor
    SerialPortReader(const std::string& port);

    // Destructor
    ~SerialPortReader();

    // Open the serial port
    bool open(int baudRate = 9600);

    // Close the serial port
    void close();

    // Check if connected
    bool isConnected() const;

    // Read a line from serial port (blocks until newline or timeout)
    bool readLine(std::string& line);

    // Check if data is available
    bool isDataAvailable();
};

#endif // SERIALPORTREADER_H
