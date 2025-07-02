// Copyright 2025 <Fedor22i>
#include <winsock2.h>
#include "Common.h"
#include "Tasks.h"
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WinsockInitializer winsock;  // Ініціалізує Winsock
    if (!winsock.isInitialized()) return 1;

    SOCKET clientSocket = createSocket();  // Створює TCP сокет
    if (clientSocket == INVALID_SOCKET) return 1;

    // Адреса сервера
    sockaddr_in serverAddr = createAddress("127.0.0.1", 54000);
    if (connect(clientSocket,
        (reinterpret_cast<sockaddr*>)&serverAddr,
        sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        return 1;
    }

    // Приймаємо вітальне повідомлення (ID клієнта)
    char welcomeBuf[128];
    int welcomeBytes = recv(clientSocket,
        welcomeBuf, sizeof(welcomeBuf) - 1, 0);
    if (welcomeBytes > 0) {
        welcomeBuf[welcomeBytes] = '\0';
        std::cout << welcomeBuf;
    }

    int taskNumber;
    std::cout << "Task 4: Find max in array\n" <<
    "Task 5: Check number against server\n" <<
    "Task 6: Check if number is a palindrome" << std::endl;
    std::cout << "Select task (4, 5, 6): ";
    std::cin >> taskNumber;

    std::string taskMessage = std::to_string(taskNumber);

    // Відправляє номер задачі на сервер
    send(clientSocket, taskMessage.c_str(), taskMessage.size(), 0);

    // Виконує відповідну клієнтську логіку
    dispatchClientTask(taskNumber, clientSocket);

    char buffer[1024];
    // Очікує відповідь від сервера
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    std::cout << "Press Enter to exit...";
    std::cin.get();
    std::cin.get();
    return 0;
}
