#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

// Функція для виконання задачі 4: Відправка масиву чисел на сервер для знаходження максимального елемента
void executeTask4(SOCKET clientSocket) {
    int arr[5];
    std::cout << "Enter 5 integers for the array: ";
    for (int i = 0; i < 5; ++i) {
        std::cin >> arr[i];  // Введення 5 чисел
    }
    send(clientSocket, (char*)arr, sizeof(arr), 0);  // Відправка масиву на сервер
    std::cout << "Array sent to server for Task 4." << std::endl;
}

// Функція для виконання задачі 5: Відправка числа на сервер для порівняння з числом на сервері
void executeTask5(SOCKET clientSocket) {
    int num;
    std::cout << "Enter a number to compare with server: ";
    std::cin >> num;  // Введення числа
    send(clientSocket, (char*)&num, sizeof(num), 0);  // Відправка числа на сервер
    std::cout << "Executing task 5..." << std::endl;
}

// Функція для виконання задачі 6: Відправка числа на сервер для перевірки, чи є воно паліндромом
void executeTask6(SOCKET clientSocket) {
    int num;
    std::cout << "Enter a number to check for palindrome: ";
    std::cin >> num;  // Введення числа
    send(clientSocket, (char*)&num, sizeof(num), 0);  // Відправка числа на сервер
    std::cout << "Executing task 6..." << std::endl;
}

// Головна функція клієнта
int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024];

    // Ініціалізація Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Створення сокета
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Налаштування адреси сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Локальний сервер

    // Підключення до сервера
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Вибір завдання
    int taskNumber;
    std::cout << "Task 4: Find max in array" << std::endl;
    std::cout << "Task 5: Check number against server" << std::endl;
    std::cout << "Task 6: Check if number is a palindrome" << std::endl;
    std::cout << "Select task (4, 5, 6): "<< std::endl;
    std::cin >> taskNumber;  // Вибір завдання

    // Відправка вибраного завдання на сервер
    std::string taskMessage = std::to_string(taskNumber);
    send(clientSocket, taskMessage.c_str(), taskMessage.size(), 0);

    // Виконання вибраного завдання
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
            std::cerr << "Invalid task number." << std::endl;
            break;
    }

    // Очікування відповіді від сервера
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Додавання символу кінця рядка
        std::cout << "Server response: " << buffer << std::endl;  // Виведення відповіді від сервера
    }

    // Очищення ресурсів
    closesocket(clientSocket);
    WSACleanup();

    // Очікування введення користувача перед закриттям програми
    std::cout << "Press Enter to exit...";
    std::cin.get();
    std::cin.get();
    return 0;
}
