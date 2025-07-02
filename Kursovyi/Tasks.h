#pragma once
#include <winsock2.h>

namespace Task4 {
    void sendFromClient(SOCKET);
    void handleOnServer(SOCKET);
}

namespace Task5 {
    void sendFromClient(SOCKET);
    void handleOnServer(SOCKET);
}

namespace Task6 {
    void sendFromClient(SOCKET);
    void handleOnServer(SOCKET);
}

void dispatchClientTask(int task, SOCKET sock);
void dispatchServerTask(int task, SOCKET sock);
