#include <iostream>
#include <string>
#include <winsock2.h>
#include <thread>
#include "Common.h"
#include "Tasks.h"

#pragma comment(lib, "ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        int taskNumber = std::stoi(buffer);
        dispatchServerTask(taskNumber, clientSocket);
    }
    closesocket(clientSocket);
    std::cout << "Client disconnected." << std::endl;
}

int main() {
    WinsockInitializer winsock;
    if (!winsock.isInitialized()) return 1;

    SOCKET serverSocket = createSocket();
    if (serverSocket == INVALID_SOCKET) return 1;

    sockaddr_in serverAddr = createServerAddress(54000);
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        return 1;
    }

    std::cout << "Server is running. Waiting for connections..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        std::cout << "Client connected." << std::endl;
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    return 0;
}
