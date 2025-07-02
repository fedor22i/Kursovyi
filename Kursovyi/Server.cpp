#include <iostream>
#include <string>
#include <winsock2.h>
#include <thread>
#include <atomic>
#include "Common.h"
#include "Tasks.h"

#pragma comment(lib, "ws2_32.lib")

// Глобальний атомарний лічильник для унікальних ID клієнтів
static std::atomic<int> clientIdCounter{1};

// Обробляє підключення клієнта, отримує ID клієнта
void handleClient(SOCKET clientSocket, int clientId) {
    std::cout << "Client #" << clientId << " connected." << std::endl;

    // (Опційно) відправити клієнту його ID
    std::string welcomeMsg = "Your client ID is " + std::to_string(clientId) + "\n";
    send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size(), 0);

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);  // Приймає номер задачі
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        int taskNumber = std::stoi(buffer);
        dispatchServerTask(taskNumber, clientSocket);
    }

    closesocket(clientSocket);
    std::cout << "Client #" << clientId << " disconnected." << std::endl;
}

int main() {
    WinsockInitializer winsock;  // Ініціалізує Winsock
    if (!winsock.isInitialized()) return 1;

    SOCKET serverSocket = createSocket();  // Створює серверний сокет
    if (serverSocket == INVALID_SOCKET) return 1;

    sockaddr_in serverAddr = createServerAddress(54000);  // Адреса сервера (INADDR_ANY)
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

        int clientId = clientIdCounter.fetch_add(1);  // Присвоюємо унікальний ID клієнту

        // Запускаємо обробку клієнта в новому потоці з clientId
        std::thread clientThread(handleClient, clientSocket, clientId);
        clientThread.detach();
    }

    closesocket(serverSocket);
    return 0;
}
