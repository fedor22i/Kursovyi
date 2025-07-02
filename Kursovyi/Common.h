// Copyright 2025 <Fedor22i>
#pragma once
#include <winsock2.h>
#include <string>

// Перевірка, чи є число паліндромом
bool isPalindrome(int number);

// Перетворення масиву цілих чисел у рядок для виводу
std::string arrayToString(const int* arr, int size);

// RAII-клас для ініціалізації Winsock (WSAStartup/WSACleanup)
class WinsockInitializer {
 public:
    WinsockInitializer();    // Ініціалізує Winsock при створенні об'єкта
    ~WinsockInitializer();   // Очищає Winsock при знищенні об'єкта
    bool isInitialized() const;  // Перевірка успішної ініціалізації
 private:
    bool initialized;
};

// Створює TCP сокет, повертає SOCKET або INVALID_SOCKET у разі помилки
SOCKET createSocket();

// Створює адресу сервера для клієнта за IP і портом
sockaddr_in createAddress(const std::string& ip, int port);

// Створює адресу сервера для bind з заданим портом (INADDR_ANY)
sockaddr_in createServerAddress(int port);
