#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SeaBattleStaticLib.cpp"
#include "framework.h"

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

void setupUserFleet(Game& game) {
    printSetupInstructions();

    std::cout << "Начнем расстановку вашего флота.\n";

    while (!game.isUserReady()) {
        std::cout << "Введите данные корабля: ";
        std::string input;
        std::getline(std::cin, input);

        // игнорируем пустые строки
        if (input.empty()) {
            continue;
        }

        try {
            // Пытаемся добавить корабль. Если формат или позиция неверны, 
            // user_add_ship выбросит std::logic_error
            game.user_add_ship(input);
            std::cout << "Корабль успешно размещен!\n";

            std::cout << "\nВаше текущее поле:\n";
            game.show_game_window_when_input();
        }
        catch (const std::logic_error& e) {
            // Ловим ошибку и показываем пользователю, что именно он сделал не так
            std::cout << "\n[ОШИБКА] " << e.what() << "\n";
            std::cout << "Попробуйте ввести координаты еще раз.\n\n";
        }
    }

    std::cout << "\nОтлично! Ваш флот полностью готов к бою.\n";
    std::cout << "Нажмите Enter, чтобы компьютер расставил свой флот и игра началась...";
    std::cin.ignore(); // Ждем нажатия Enter
}


void runInteractiveMatch(Game& game) {
    std::cout << "\nИгра началась!\n";

    while (!game.is_end()) {
        //Ход пользователя
        bool userHit = false;
        do {
            std::cout << "\n--- Ваш ход ---\n";
            std::cout << "Введите координаты выстрела (строка колонка, например '1 A'): ";
            std::string input;
            std::getline(std::cin, input);

            try {
                State state = game.user_move(input);
                if (state == Missed) {
                    std::cout << "Промах!\n";
                    userHit = false;
                    clearScreen();
                }
                else {
                    std::cout << "Попадание!\n";
                    userHit = true;
                }
            }
            catch (const std::logic_error& e) {
                std::cout << "[ОШИБКА] " << e.what() << "\n";
                continue; // Не прерываем цикл, даем пользователю ввести координаты заново
            }
            clearScreen();
            game.show_game_window(); // Обновляем экран после каждого действия

        } while (userHit && !game.is_end()); // Если попал, ход повторяется

        if (game.is_end()) break;

        //Ход компьютера
        bool compHit = false;
        do {
            std::cout << "\n--- Ход компьютера ---\n";
            // Небольшая задержка для реалистичности (опционально)
            std::this_thread::sleep_for(std::chrono::milliseconds(800));

            State state = game.computer_move();

            if (state == Missed) {
                std::cout << "Компьютер промахнулся.\n";
                compHit = false;
            }
            else {
                std::cout << "Компьютер попал!\n";
                compHit = true;
            }

            game.show_game_window(); // Обновляем экран

        } while (compHit && !game.is_end()); // Если компьютер попал, он ходит снова
    }

    //завершение игры
    std::cout << "\n=================================================\n";
    if (game.isUserWinner()) {
        std::cout << "           ПОЗДРАВЛЯЕМ! ВЫ ПОБЕДИЛИ!           \n";
    }
    else {
        std::cout << "          КОМПЬЮТЕР ПОБЕДИЛ. ПОПРОБУЙТЕ ЕЩЕ!   \n";
    }
    std::cout << "=================================================\n";
}


//Главная функция запуска
int SeaBattleGameFunction() {
    try {
        Game game;

        // Расстановка флота пользователя (интерактивно, с проверками)
        setupUserFleet(game);

        // Случайная расстановка флота компьютера
        // Передаем пустую строку, чтобы сработала логика random из computer_init
        game.computer_init("");

        // Запуск игрового цикла
        runInteractiveMatch(game);

        return 0; // Успешное завершение

    }
    catch (const std::exception& e) {
        std::cerr << "\nКритическая ошибка: " << e.what() << "\n";
        return 1; // Завершение с кодом ошибки
    }
}
