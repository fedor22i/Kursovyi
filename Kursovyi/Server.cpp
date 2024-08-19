#include <iostream>
#include <string>
#include <winsock2.h>
#include <algorithm>
#include <sstream>
#include <thread>  // Додаємо підтримку потоків

#pragma comment(lib, "ws2_32.lib")

// Функція для виконання задачі 4: Знайти максимальне значення в масиві
void executeTask4(SOCKET clientSocket) {
    int arr[5];
    recv(clientSocket, (char*)arr, sizeof(arr), 0);  // Отримуємо масив від клієнта

    int maxVal = *std::max_element(arr, arr + 5);  // Знаходимо максимальне значення в масиві
    std::ostringstream oss;
    oss << "Max value in array: " << maxVal;
    std::string response = oss.str();
    send(clientSocket, response.c_str(), response.size(), 0);  // Відправляємо результат клієнту
}

// Функція для виконання задачі 5: Порівняти число клієнта з числом на сервері
void executeTask5(SOCKET clientSocket) {
    int clientNum;
    recv(clientSocket, (char*)&clientNum, sizeof(clientNum), 0);  // Отримуємо число від клієнта
    int serverNum = 10;  // Число на сервері для порівняння
    std::string response = (clientNum > serverNum) ? "Client number is greater" : "Server number is greater or equal";
    send(clientSocket, response.c_str(), response.size(), 0);  // Відправляємо результат клієнту
}

// Функція для виконання задачі 6: Перевірити, чи є число паліндромом
void executeTask6(SOCKET clientSocket) {
    int num;
    recv(clientSocket, (char*)&num, sizeof(num), 0);  // Отримуємо число від клієнта
    
    int original = num, reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + num % 10;  // Реверсуємо число
        num /= 10;
    }
    
    std::string response = (original == reversed) ? "Number is a palindrome" : "Number is not a palindrome";
    send(clientSocket, response.c_str(), response.size(), 0);  // Відправляємо результат клієнту
}

// Функція для обробки клієнта
void handleClient(SOCKET clientSocket) {
    char buffer[1024];

    // Отримуємо номер завдання від клієнта
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        int taskNumber = std::stoi(buffer);

        // Виконуємо обрану задачу
        switch (taskNumber) {
            case 4:
                executeTask4(clientSocket);
                break;
            case 5:
                executeTask5(clientSocket);
                break;
            case 6:
                executeTask6(clientSocket);
                break;
            default:
                std::cerr << "Invalid task number received." << std::endl;
                break;
        }
    }

    // Закриваємо сокет клієнта після обробки
    closesocket(clientSocket);
    std::cout << "Client disconnected." << std::endl;
}

// Головна функція сервера
int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Ініціалізація Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Створюємо сокет
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Налаштовуємо адресу сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Прив'язуємо сокет до адреси
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Запускаємо прослуховування з'єднань
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is running. Waiting for connections..." << std::endl;

    // Головний цикл для підтримки роботи сервера
    while (true) {
        // Приймаємо підключення від клієнта
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Client connected." << std::endl;

        // Створюємо новий потік для обробки клієнта
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Від'єднуємо потік, щоб він працював самостійно
    }

    // Очищення ресурсів
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
