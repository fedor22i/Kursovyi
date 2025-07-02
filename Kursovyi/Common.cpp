// Copyright 2025 <Fedor22i>
#include "Common.h"
#include <iostream>
#include <string>

// Перевіряє, чи є число паліндромом
bool isPalindrome(int number) {
    int original = number, reversed = 0;
    while (number > 0) {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    }
    return original == reversed;
}

// Формує рядок з елементів масиву для виводу
std::string arrayToString(const int* arr, int size) {
    std::string result = "[";
    for (int i = 0; i < size; ++i) {
        result += std::to_string(arr[i]);
        if (i < size - 1) result += ", ";
    }
    result += "]";
    return result;
}

// Ініціалізує Winsock (WSAStartup)
WinsockInitializer::WinsockInitializer() {
    WSADATA wsaData;
    initialized = (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
    if (!initialized) {
        std::cerr << "WSAStartup failed." << std::endl;
    }
}

// Очищає Winsock (WSACleanup)
WinsockInitializer::~WinsockInitializer() {
    if (initialized) WSACleanup();
}

// Повертає true, якщо Winsock успішно ініціалізовано
bool WinsockInitializer::isInitialized() const {
    return initialized;
}

// Створює TCP сокет, повертає INVALID_SOCKET при помилці
SOCKET createSocket() {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
    }
    return s;
}

// Створює адресу сервера з IP і портом
sockaddr_in createAddress(const std::string& ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    return addr;
}

// Створює адресу сервера для bind (INADDR_ANY)
sockaddr_in createServerAddress(int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}
