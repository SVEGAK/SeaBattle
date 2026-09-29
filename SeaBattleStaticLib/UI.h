#pragma once
#include <iostream>
#include <thread>
#include <chrono>

void clearScreen() {
#ifdef _WIN32
    system("cls");      // Для Windows
#else
    system("clear");    // Для Linux / macOS
#endif
}
//Пауза
void waitSeconds(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
void printSetupInstructions() {
    system("chcp 65001");system("cls");
    std::cout << "=================================================\n";
    std::cout << "          ДОБРО ПОЖАЛОВАТЬ В МОРСКОЙ БОЙ         \n";
    std::cout << "=================================================\n";
    std::cout << "Вам нужно расставить 10 кораблей:\n";
    std::cout << "  - 1 четырёхпалубный (****)\n";
    std::cout << "  - 2 трёхпалубных (***), (***)\n";
    std::cout << "  - 3 двухпалубных (**), (**), (**)\n";
    std::cout << "  - 4 однопалубных (*), (*), (*), (*)\n\n";
    std::cout << "Формат ввода: размер строка колонка направление\n";
    std::cout << "Пример: 4 1 A H  (4-палубный, 1-я строка, колонка A, горизонтально)\n";
    std::cout << "Важно: Корабли не должны касаться друг друга (даже углами)!\n";
    std::cout << "=================================================\n\n";
}
