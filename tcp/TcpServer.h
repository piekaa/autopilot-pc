#ifndef MSFS_CONTROLLER_TCPSERVER_H
#define MSFS_CONTROLLER_TCPSERVER_H

#include <winsock2.h>
#include <string>
#include <iostream>
#include <optional>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
    SOCKET listenSocket;
    SOCKET clientSocket;
    bool connected;
    int port;
    std::string buffer;

    // Heartbeat tracking
    std::chrono::steady_clock::time_point lastPingSent;
    std::chrono::steady_clock::time_point lastPongReceived;
    bool waitingForPong;

    void initializeWinsock() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed with error: " << result << std::endl;
            throw std::runtime_error("Failed to initialize Winsock");
        }
    }

    void createListenSocket() {
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            throw std::runtime_error("Failed to create socket");
        }

        // Set socket to non-blocking mode
        u_long mode = 1;
        ioctlsocket(listenSocket, FIONBIO, &mode);

        // Bind to port
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            throw std::runtime_error("Failed to bind socket");
        }

        // Listen for connections
        if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            throw std::runtime_error("Failed to listen on socket");
        }

        std::cout << "TCP Server listening on port " << port << std::endl;
    }

    void acceptClient() {
        if (connected) {
            return;
        }

        SOCKET newClient = accept(listenSocket, nullptr, nullptr);
        if (newClient != INVALID_SOCKET) {
            clientSocket = newClient;

            // Set client socket to non-blocking mode
            u_long mode = 1;
            ioctlsocket(clientSocket, FIONBIO, &mode);

            connected = true;
            waitingForPong = false;
            lastPingSent = std::chrono::steady_clock::now();
            lastPongReceived = std::chrono::steady_clock::now();
            std::cout << "Client connected to TCP server" << std::endl;
        }
    }

    void disconnectClient() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }
        connected = false;
        waitingForPong = false;
        buffer.clear();
        std::cout << "Client disconnected, awaiting new client connection..." << std::endl;
    }

    void checkHeartbeat() {
        if (!connected) {
            return;
        }

        auto now = std::chrono::steady_clock::now();

        // Check if we need to send PING (every 5 seconds)
        auto timeSinceLastPing = std::chrono::duration_cast<std::chrono::seconds>(now - lastPingSent);
        if (timeSinceLastPing.count() >= 5 && !waitingForPong) {
            // Send PING PING
            if (sendRaw("PING PING\n")) {
                lastPingSent = now;
                waitingForPong = true;
                std::cout << "Sent PING PING" << std::endl;
            }
        }

        // Check if PONG timeout occurred (3 seconds after PING)
        if (waitingForPong) {
            auto timeSincePing = std::chrono::duration_cast<std::chrono::seconds>(now - lastPingSent);
            if (timeSincePing.count() >= 3) {
                std::cout << "PONG timeout - disconnecting client" << std::endl;
                disconnectClient();
            }
        }
    }

    bool sendRaw(const std::string& data) {
        if (!connected || clientSocket == INVALID_SOCKET) {
            return false;
        }

        int bytesSent = send(clientSocket, data.c_str(), data.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            int error = WSAGetLastError();
            std::cerr << "send failed with error: " << error << std::endl;
            disconnectClient();
            return false;
        }

        return bytesSent == data.length();
    }

public:
    TcpServer(int serverPort = 5000) : listenSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET),
                                        connected(false), port(serverPort), waitingForPong(false) {
        try {
            initializeWinsock();
            createListenSocket();
        } catch (const std::exception& e) {
            std::cerr << "TcpServer initialization failed: " << e.what() << std::endl;
        }
    }

    ~TcpServer() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        if (listenSocket != INVALID_SOCKET) {
            closesocket(listenSocket);
        }
        WSACleanup();
    }

    // For compatibility with Serial.h write method
    SOCKET getHandle() const {
        return clientSocket;
    }

    bool isConnected() {
        // Try to accept new clients if not connected
        if (!connected) {
            acceptClient();
        } else {
            checkHeartbeat();
        }
        return connected;
    }

    // Read a line from TCP socket (until '\n')
    // Returns nullopt immediately if no complete line is available (non-blocking)
    std::optional<std::string> readLine() {
        if (!isConnected()) {
            return std::nullopt;
        }

        // Check if we already have a complete line in the buffer
        size_t newlinePos = buffer.find('\n');
        if (newlinePos != std::string::npos) {
            std::string line = buffer.substr(0, newlinePos);
            buffer.erase(0, newlinePos + 1);

            // Remove trailing '\r' if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            // Check if this is a PONG PONG response
            if (line == "PONG PONG") {
                std::cout << "Received PONG PONG" << std::endl;
                waitingForPong = false;
                lastPongReceived = std::chrono::steady_clock::now();
                return std::nullopt; // Don't pass PONG to application layer
            }

            return line;
        }

        // Try to read more data from socket
        char tempBuffer[256];
        int bytesRead = recv(clientSocket, tempBuffer, sizeof(tempBuffer) - 1, 0);

        if (bytesRead > 0) {
            tempBuffer[bytesRead] = '\0';
            buffer.append(tempBuffer, bytesRead);
        } else if (bytesRead == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                std::cerr << "recv failed with error: " << error << std::endl;
                disconnectClient();
                return std::nullopt;
            }
        } else if (bytesRead == 0) {
            // Client closed connection gracefully
            std::cout << "Client closed connection" << std::endl;
            disconnectClient();
            return std::nullopt;
        }

        // Check again if we now have a complete line
        newlinePos = buffer.find('\n');
        if (newlinePos != std::string::npos) {
            std::string line = buffer.substr(0, newlinePos);
            buffer.erase(0, newlinePos + 1);

            // Remove trailing '\r' if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            // Check if this is a PONG PONG response
            if (line == "PONG PONG") {
                std::cout << "Received PONG PONG" << std::endl;
                waitingForPong = false;
                lastPongReceived = std::chrono::steady_clock::now();
                return std::nullopt; // Don't pass PONG to application layer
            }

            return line;
        }

        return std::nullopt;
    }

    bool write(const std::string& data) {
        return sendRaw(data);
    }
};

#endif //MSFS_CONTROLLER_TCPSERVER_H
