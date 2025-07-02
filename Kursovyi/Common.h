#pragma once
#include <string>
#include <winsock2.h>

bool isPalindrome(int number);
std::string arrayToString(const int* arr, int size);

class WinsockInitializer {
public:
    WinsockInitializer();
    ~WinsockInitializer();
    bool isInitialized() const;
private:
    bool initialized;
};

SOCKET createSocket();
sockaddr_in createAddress(const std::string& ip, int port);
sockaddr_in createServerAddress(int port);
