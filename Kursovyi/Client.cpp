#include <iostream>
#include <string>
#include <winsock2.h>
#include "Common.h"
#include "Tasks.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    WinsockInitializer winsock;  // Ініціалізує Winsock
    if (!winsock.isInitialized()) return 1;

    SOCKET clientSocket = createSocket();  // Створює TCP сокет
    if (clientSocket == INVALID_SOCKET) return 1;

    sockaddr_in serverAddr = createAddress("127.0.0.1", 54000);  // Адреса сервера
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        return 1;
    }

    // Приймаємо вітальне повідомлення (ID клієнта)
    char welcomeBuf[128];
    int welcomeBytes = recv(clientSocket, welcomeBuf, sizeof(welcomeBuf) - 1, 0);
    if (welcomeBytes > 0) {
        welcomeBuf[welcomeBytes] = '\0';
        std::cout << welcomeBuf;
    }

    int taskNumber;
    std::cout << "Task 4: Find max in array\nTask 5: Check number against server\nTask 6: Check if number is a palindrome" << std::endl;
    std::cout << "Select task (4, 5, 6): ";
    std::cin >> taskNumber;

    std::string taskMessage = std::to_string(taskNumber);
    send(clientSocket, taskMessage.c_str(), taskMessage.size(), 0);  // Відправляє номер задачі на сервер

    dispatchClientTask(taskNumber, clientSocket);  // Виконує відповідну клієнтську логіку

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);  // Очікує відповідь від сервера
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
