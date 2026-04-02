#ifndef MSFS_CONTROLLER_TCPSERVER_H
#define MSFS_CONTROLLER_TCPSERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <optional>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
    SOCKET listenSocket;
    SOCKET clientSocket;
    bool connected;
    int port;
    std::string buffer;

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
            std::cout << "Client connected to TCP server" << std::endl;
        }
    }

    void checkClientConnection() {
        if (!connected) {
            return;
        }

        // Check if client is still connected by trying to peek at the socket
        char testBuf[1];
        int result = recv(clientSocket, testBuf, 0, 0);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                std::cout << "Client disconnected" << std::endl;
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                connected = false;
                buffer.clear();
            }
        }
    }

public:
    TcpServer(int serverPort = 5000) : listenSocket(INVALID_SOCKET), clientSocket(INVALID_SOCKET),
                                        connected(false), port(serverPort) {
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
            checkClientConnection();
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
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                connected = false;
                buffer.clear();
                return std::nullopt;
            }
        } else if (bytesRead == 0) {
            // Client closed connection gracefully
            std::cout << "Client closed connection" << std::endl;
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
            connected = false;
            buffer.clear();
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

            return line;
        }

        return std::nullopt;
    }

    bool write(const std::string& data) {
        if (!isConnected()) {
            return false;
        }

        int bytesSent = send(clientSocket, data.c_str(), data.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            int error = WSAGetLastError();
            std::cerr << "send failed with error: " << error << std::endl;
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
            connected = false;
            buffer.clear();
            return false;
        }

        return bytesSent == data.length();
    }
};

#endif //MSFS_CONTROLLER_TCPSERVER_H
