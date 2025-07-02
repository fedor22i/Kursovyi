// Copyright 2025 <Fedor22i>
#include "Tasks.h"
#include "Common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

namespace Task4 {
void sendFromClient(SOCKET sock) {
    int arr[5];
    std::cout << "Enter 5 integers for the array: ";
    for (int i = 0; i < 5; ++i) std::cin >> arr[i];
    send(sock, (reinterpret_cast<char*>)arr, sizeof(arr), 0);
    std::cout << "Array sent to server: " << arrayToString(arr, 5)
    << std::endl;
}

void handleOnServer(SOCKET sock) {
    int arr[5];
    recv(sock, (reinterpret_cast<char*>)arr, sizeof(arr), 0);
    int maxVal = *std::max_element(arr, arr + 5);
    std::ostringstream oss;
    oss << "Max value in array: " << maxVal;
    std::string response = oss.str();
    send(sock, response.c_str(), response.size(), 0);
}
}  // namespace Task4

namespace Task5 {
void sendFromClient(SOCKET sock) {
    int num;
    std::cout << "Enter a number to compare with server: ";
    std::cin >> num;
    send(sock, (reinterpret_cast<char*>)&num, sizeof(num), 0);
}

void handleOnServer(SOCKET sock) {
    int clientNum;
    recv(sock, (reinterpret_cast<char*>)&clientNum, sizeof(clientNum), 0);
    int serverNum = 10;
    std::string response = (clientNum > serverNum)
        ? "Client number is greater"
        : "Server number is greater or equal";
    send(sock, response.c_str(), response.size(), 0);
}
}  // namespace Task5

namespace Task6 {
void sendFromClient(SOCKET sock) {
    int num;
    std::cout << "Enter a number to check for palindrome: ";
    std::cin >> num;
    send(sock, (reinterpret_cast<char*>)&num, sizeof(num), 0);
}

void handleOnServer(SOCKET sock) {
    int num;
    recv(sock, (reinterpret_cast<char*>)&num, sizeof(num), 0);
    std::string response = isPalindrome(num)
        ? "Number is a palindrome"
        : "Number is not a palindrome";
    send(sock, response.c_str(), response.size(), 0);
}
}  // namespace Task6

void dispatchClientTask(int task, SOCKET sock) {
    switch (task) {
        case 4: Task4::sendFromClient(sock); break;
        case 5: Task5::sendFromClient(sock); break;
        case 6: Task6::sendFromClient(sock); break;
        default: std::cerr << "Invalid task number." << std::endl; break;
    }
}

void dispatchServerTask(int task, SOCKET sock) {
    switch (task) {
        case 4: Task4::handleOnServer(sock); break;
        case 5: Task5::handleOnServer(sock); break;
        case 6: Task6::handleOnServer(sock); break;
        default: std::cerr << "Invalid task number received." << std::endl;
        break;
    }
}
