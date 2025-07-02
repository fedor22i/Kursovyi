#pragma once
#include <winsock2.h>

namespace Task4 {
    // Клієнтська частина: вводить і відправляє масив із 5 цілих чисел
    void sendFromClient(SOCKET);

    // Серверна частина: приймає масив, знаходить максимум і відсилає результат
    void handleOnServer(SOCKET);
}

namespace Task5 {
    // Клієнтська частина: вводить і відправляє число для порівняння
    void sendFromClient(SOCKET);

    // Серверна частина: порівнює число з серверним, відправляє результат
    void handleOnServer(SOCKET);
}

namespace Task6 {
    // Клієнтська частина: вводить і відправляє число для перевірки паліндрома
    void sendFromClient(SOCKET);

    // Серверна частина: приймає число, перевіряє паліндром, відсилає відповідь
    void handleOnServer(SOCKET);
}

// Диспетчер на клієнті: виконує відповідну функцію відправки за номером задачі
void dispatchClientTask(int task, SOCKET sock);

// Диспетчер на сервері: виконує відповідну функцію обробки за номером задачі
void dispatchServerTask(int task, SOCKET sock);
