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
    std::cout << "          WELCOME TO SEABATTLE                   \n";
    std::cout << "=================================================\n";
    std::cout << "You need to place 10 ships:\n";
    std::cout << "  - 1 four-deck ship (****)\n";
    std::cout << "  - 2 three-deck ships (***), (***)\n";
    std::cout << "  - 3 two-deck ships (**), (**), (**)\n";
    std::cout << "  - 4 one-deck ships (*), (*), (*), (*)\n\n";
    std::cout << "Input format: size row column direction\n";
    std::cout << "Example: 4 1 A H  (4-deck, row 1, column A, horizontally)\n";
    std::cout << "Important: Ships must not touch each other (even at the corners)!\n";
    std::cout << "=================================================\n\n";
}