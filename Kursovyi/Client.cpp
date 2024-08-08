#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void executeTask4(SOCKET clientSocket) {
    int arr[5];
    std::cout << "Enter 5 integers for the array: ";
    for (int i = 0; i < 5; ++i) {
        std::cin >> arr[i];
    }
    send(clientSocket, (char*)arr, sizeof(arr), 0);
    std::cout << "Array sent to server for Task 4." << std::endl;
}

void executeTask5(SOCKET clientSocket) {
    int num;
    std::cout << "Enter a number to compare with server: ";
    std::cin >> num;
    send(clientSocket, (char*)&num, sizeof(num), 0);
    std::cout << "Executing task 5..." << std::endl;
}

void executeTask6(SOCKET clientSocket) {
    int num;
    std::cout << "Enter a number to check for palindrome: ";
    std::cin >> num;
    send(clientSocket, (char*)&num, sizeof(num), 0);
    std::cout << "Executing task 6..." << std::endl;
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Choose task
    int taskNumber;
    std::cout << "Task 4: Find max in array" << std::endl;
    std::cout << "Task 5: Check number against server" << std::endl;
    std::cout << "Task 6: Check if number is a palindrome" << std::endl;
    std::cout << "Select task (4, 5, 6): "<< std::endl;
    std::cin >> taskNumber;

    // Send selected task to server
    std::string taskMessage = std::to_string(taskNumber);
    send(clientSocket, taskMessage.c_str(), taskMessage.size(), 0);

    // Execute the chosen task
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

    // Wait for server response
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Clean up
    closesocket(clientSocket);
    WSACleanup();

    // Wait for user input before closing
    std::cout << "Press Enter to exit...";
    std::cin.get();
    std::cin.get();
    return 0;
}
