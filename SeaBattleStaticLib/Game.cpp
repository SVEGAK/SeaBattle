
#include "Game.h"
void Game::user_add_ship(const std::string& input) {
    std::istringstream iss(input);
    int size, row;
    char col, dir;

    // проверяем что ввод не пустой
    if (input.empty() || iss.fail()) {
        throw std::logic_error("Empty input. Format: size string column direction (For example: 1 1 A H)");
    }

    // пытаемся считать параметры
    if (!(iss >> size >> row >> col >> dir)) {
        throw std::logic_error("Incorrect input format. Except: size string column direction (For example: 1 1 A H)");
    }

    std::string remaining;
    if (iss >> remaining) {
        throw std::logic_error("Excess data after the ship. Enter only one ship at a time.");
    }

    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid ship size: " + std::to_string(size) + ". Acceptable values: 1-4");
    }

    char dir_upper = std::toupper(static_cast<unsigned char>(dir));
    if (dir_upper != 'H' && dir_upper != 'V') {
        throw std::logic_error("Invalid direction: '" + std::string(1, dir) + "'. Use ‘H’ (horizontally) or ‘V’ (vertically).");
    }

    if (row < 1 || row > 10) {
        throw std::logic_error("Invalid line number: " + std::to_string(row) + ". Acceptable values: 1–10");
    }

    char col_upper = std::toupper(static_cast<unsigned char>(col));
    if (col_upper < 'A' || col_upper > 'J') {
        throw std::logic_error("Invalid column: '" + std::string(1, col) + "'. Valid values: A-J");
    }

    try {
        _user.set_ship(Ship(size, dir_upper, row, col_upper));
    }
    catch (const std::out_of_range& e) {
        throw std::logic_error("The ship is going beyond the field boundaries. Check the coordinates and direction.");
    }
    catch (const std::invalid_argument& e) {
        throw std::logic_error("The ship intersects or touches another ship. Choose another position.");
    }
    catch (const std::logic_error& e) {
        throw; // пробрасываем остальное
    }
}

void Game::user_init(const std::string& input) {
    std::istringstream iss(input);
    int size, row;
    char col, dir;

    int ship_count = 0;

    // Ожидаем формат: "размер строка колонка направление" для каждого корабля
    while (iss >> size >> row >> col >> dir) {
        ship_count++;

        try {
            _user.set_ship(Ship(size, dir, row, col));
        }
        catch (const std::logic_error& e) {
            throw std::logic_error("Eror while adding a ship #" + std::to_string(ship_count) +
                " (" + std::to_string(size) + " " + std::to_string(row) +
                " " + col + " " + dir + "): " + e.what());
        }
    }

    // Проверяем, что были введены данные
    if (ship_count == 0) {
        throw std::logic_error("No one ship had been added");
    }

    // Проверяем готовность флота
    if (!_user.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}



void Game::computer_init(const std::string& input) {
    if (!input.empty()) {//Если будет передана не пустая строка - заполняем по ней
        std::istringstream iss(input);
        int size, row;
        char col, dir;
        while (iss >> size >> row >> col >> dir) {
            _computer.set_ship(Ship(size, dir, row, col));
        }
        if (!_computer.check_ready()) {
            throw std::logic_error("Invalid input: incorrect field");
        }
        return;
    }

    // Случайная расстановка
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> row_dist(1, 10); //гереатор строки (1 - 10) 
    std::uniform_int_distribution<int> col_dist(0, 9); //генератор колонок
    std::uniform_int_distribution<int> dir_dist(0, 1); //генератор направления

    const char directions[] = { 'H', 'V' };

    // конфигурация флота: {размер, количество}
    const std::vector<std::pair<int, int>> fleet_config = {
        {4, 1}, {3, 2}, {2, 3}, {1, 4}
    };

    for (const auto& config : fleet_config) {
        int size = config.first;
        int count = config.second;

        for (int i = 0; i < count; i++) {
            bool placed = false;
            int attempts = 0;
            const int max_attempts = 10000; // количество попыток ограничено

            while (!placed && attempts < max_attempts) {
                int r = row_dist(gen);
                char c = static_cast<char>('A' + col_dist(gen));
                char d = directions[dir_dist(gen)];

                try {
                    // Пытаемся поставить корабль. 
                    // Если он вылезает за границы или касается других, set_ship выбросит исключение,
                    // которое мы ловим и просто пробуем новые координаты.
                    _computer.set_ship(Ship(size, d, r, c));
                    placed = true;
                }
                catch (const std::logic_error&) {
                    attempts++;
                }
            }

            // Если за 10000 попыток не удалось поставить корабль
            if (!placed) {
                throw std::logic_error("Invalid input: incorrect field");
            }
        }
    }
}



State Game::user_move(const std::string& input) {
    std::istringstream iss(input);
    int row;
    char col;

    if (!(iss >> row >> col)) {
        throw std::logic_error("Invalid input: incorrect move");
    }

    return _computer.set_action(row, col);
}

State Game::computer_move() {
    // левая диагональ
    for (int i = 1; i <= 10; i++) {
        char c = static_cast<char>('A' + i - 1);
        try {
            return _user.set_action(i, c);
        }
        catch (...) {
            // клетка уже обстреляна
        }
    }

    // правая диагональ 
    for (int i = 1; i <= 10; i++) {
        char c = static_cast<char>('A' + (10 - i));
        try {
            return _user.set_action(i, c);
        }
        catch (...) {
            // Клетка уже обстреляна
        }
    }

    //оставшиеся ячейки подряд рядами
    for (int r = 1; r <= 10; r++) {
        for (char c = 'A'; c <= 'J'; c++) {
            try {
                return _user.set_action(r, c);
            }
            catch (...) {
                // Клетка уже обстреляна
            }
        }
    }

    throw std::logic_error("Invalid input: incorrect move");
}



bool Game::is_end() const noexcept {
    // Вернет true, только если один true, а другой false
    return _user.check_lose() != _computer.check_lose();
}
void Game::show_game_window() const {
    std::cout << "= COMPUTER GAME FIELD =\n\n";
    _computer.show_field(true, false); // true = скрыть корабли

    std::cout << "\n=== YOUR PLAY FIELD ===\n\n";
    _user.show_field(false, false); // false = показать корабли
    std::cout << std::endl;
}

void Game::show_game_window_when_input() const {
    std::cout << "\n=== YOUR PLAY FIELD ===\n\n";
    _user.show_field(false, true); // false = показать корабли
    std::cout << std::endl;
}

void Game::start() {
    std::string user_input, comp_input, dummy;

    std::getline(std::cin, user_input);
    std::getline(std::cin, dummy);
    std::getline(std::cin, comp_input);

    try {
        user_init(user_input);
        computer_init(comp_input);
    }
    catch (const std::logic_error& e) {
        std::cerr << e.what() << "\n";
        return;
    }

    show_game_window();


    while (!is_end()) {

        while (!is_end()) {
            std::string move_input;
            std::getline(std::cin, move_input);

            State u_state = user_move(move_input);
            show_game_window();

            if (u_state == Missed) {
                break; // передаем ход пк
            }

        }

        if (is_end()) break;

        while (!is_end()) {
            State c_state = computer_move();
            show_game_window();

            if (c_state == Missed) {
                break; // передаем ход юзеру
            }

        }
    }

    show_game_window();
    if (_computer.check_lose()) {
        std::cout << "USER WIN!\n";
    }
    else {
        std::cout << "COMPUTER WIN!\n";
    }
}