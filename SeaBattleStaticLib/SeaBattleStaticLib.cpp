// SeaBattleStaticLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

Position::Position(int row, int col) : _row(row), _col(col) {
    if (row < 1 || row > _max_row || col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
}
Position::Position(const std::string& str) {
    int row, col;
    row = 1;
    col = 1;
    std::istringstream iss(str);
    char ch1, ch2, ch3;

    iss >> ch1 >> row >> ch2 >> col >> ch3;

    if (ch1 != '(' || ch2 != ',' || ch3 != ')' || iss.fail()) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    if (row < 1 || row > _max_row || col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    _row = row;
    _col = col;
}
void Position::row(int new_row) {
    if (new_row < 1 || new_row > _max_row) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _row = new_row;
}
void Position::col(int new_col) {
    if (new_col < 1 || new_col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _col = new_col;
}
std::string to_string(const Position& pos) {
    std::ostringstream oss;
    oss << "(" << pos._row << ", " << pos._col << ")";
    return oss.str();
}

Position parse(const std::string& str) {
    std::istringstream iss(str);
    char ch1, ch2, ch3;
    int row, col;

    iss >> ch1 >> row >> ch2 >> col >> ch3;

    if (ch1 != '(' || ch2 != ',' || ch3 != ')' || iss.fail()) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    if (row < 1 || row > Position::_max_row || col < 1 || col > Position::_max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    return Position(row, col);
}

Ship::Ship(int size, Position coord, Direction dir) {
    if ((size > 0) && (size <= 4)) {
        _size = size;
    }
    else { throw std::logic_error("Invalid input: incorrect ship parameters"); }
    if (!is_collision(size, coord, dir)) {
        _coord = coord;
        _direction = dir;
    }
    else { throw std::logic_error("Invalid input: incorrect ship parameters"); }
}
Ship::Ship(int size, char dir, int row, char col) :
    Ship(size, Position(row, std::toupper(col) - 'A' + 1),
        (std::toupper(dir) == 'H' ? Horizontal
            : std::toupper(dir) == 'V' ? Vertical
            : throw std::logic_error("Invalid input: incorrect ship parameters"))) {
}
Ship::Ship(const Ship& other): _size(other.size()),_coord(other._coord),_direction(other._direction){}
bool Ship::is_collision(int size, const Position& coord, Direction dir) const noexcept { //True - выходит за пределы поля
    if (dir == Horizontal) {
        //Вертикальное положение корабля проверяется в конструкторе Position
        if ((coord.col() < 1) || (coord.col() + (size - 1) > Position::_max_col)) {
            return true;
        }
        return false;
    }
    if (dir == Vertical) {
        //Часть корректности вертикального положения проверяется в конструкторе Position
        if ((coord.row() + (size - 1)) > Position::_max_row) {
            return true;
        }
        else if ((coord.col() < 1) || (coord.col() > Position::_max_col)) {
            return true;
        }
        return false;
    }
    return true;
}



void Ship::rotate() {
    Direction next_dir = (_direction == Horizontal) ? Vertical : Horizontal;

    if (is_collision(_size, _coord, next_dir)) {
        throw std::logic_error("Invalid input: incorrect ship parameters");
    }

    _direction = next_dir;
}



void  GameField::check_dimensions(int n, int m) const {
    if (n <= 0 || n > 25 || m <= 0 || m > 25) {
        throw std::logic_error("Invalid input: incorrect field parameters");
    }
}
void GameField::check_position(int row, char col) const {
    if (row < 1 || row > _n || col < 'A' || col >= 'A' + _m) {
        throw std::logic_error("Invalid input: incorrect position");
    }
}
void GameField::allocate_and_fill(char fill_char) {
    _field = new char* [_n];
    for (int i = 0; i < _n; i++) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = fill_char;
        }
    }
}

void GameField::copy_from(const GameField& other) {
    _field = new char* [_n];
    for (int i = 0; i < _n; i++) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = other._field[i][j];
        }
    }
}
GameField::GameField(char** field, int n, int m) : _n(n), _m(m) {
    check_dimensions(n, m);
    _field = new char* [_n];
    for (int i = 0; i < _n; i++) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = field[i][j];
        }
    }
}

std::string to_string(const GameField& gf, bool hide_ships = false) {
    std::string res;

    // верхняя строка: "  |A B C ...|"
    res += "  |";
    for (int j = 0; j < gf._m; ++j) {
        res += static_cast<char>('A' + j);
        if (j < gf._m - 1) {
            res += " ";
        }
    }
    res += "|\n";

    // верхняя граница: "  +-------+"
    res += "  +";
    res += std::string(gf._m * 2 - 1, '-');
    res += "+\n";

    // строки поля
    for (int i = 0; i < gf._n; i++) {
        res += std::to_string(i + 1) + " |";
        for (int j = 0; j < gf._m; ++j) {
            char c = gf._field[i][j];

            //скрываем корабли если нужно
            if (hide_ships && c == '*') {
                c = ' '; 
            }

            res += c;

            if (j < gf._m - 1) {
                res += " ";
            }
        }
        res += "|\n";
    }

    // нижняя граница 
    res += "  +";
    res += std::string(gf._m * 2 - 1, '-');
    res += "+";

    return res;
}
const int Player::_max_ships_counts[4] = { 4, 3, 2, 1 };


bool Player::checkCollision(const Ship& ship) const {
    for (int i = 0; i < ship.size(); i++) {
        int r = ship.row() + (ship.direction() == Vertical ? i : 0);
        int c = ship.col() + (ship.direction() == Horizontal ? i : 0);

        // проверяем саму клетку и все 8 соседних (правило: корабли не касаются)
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int check_r = r + dr;
                char check_c = static_cast<char>('A' + (c - 1) + dc);
                try {
                    if (_gamefield.get(check_r, check_c) != ' ') {
                        return true;
                    }
                }
                catch (...) {
                    // выход за границы поля при проверке окрестности игнорируем
                }
            }
        }
    }
    return false;
}



Player::Player() : _gamefield(), _ships_counts{ 0, 0, 0, 0 } {}

void Player::set_ship(const Ship& ship) {
    int size = ship.size();

    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    if (_ships_counts[size - 1] >= _max_ships_counts[size - 1]) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    if (checkCollision(ship)) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    // размещение корабля на поле
    for (int i = 0; i < size; i++) {
        int r = ship.row() + (ship.direction() == Vertical ? i : 0);
        int c = ship.col() + (ship.direction() == Horizontal ? i : 0);
        char col_char = static_cast<char>('A' + c - 1);

        _gamefield.set(r, col_char);
    }

    _ships_counts[size - 1]++;
    _ships.push_back(ship);
}


State Player::set_action(int row, char col) {
    char current = ' ';
    try {
        current = _gamefield.get(row, col);
    }
    catch (...) {
        throw std::logic_error("Invalid input: incorrect move");
    }

    if (current == 'X' || current == '.') {
        throw std::logic_error("Invalid input: incorrect move");
    }

    if (current == ' ') {
        _gamefield.set_char(row, col, '.'); 
        return Missed;
    }

    if (current == '*') {
        _gamefield.set_char(row, col, 'X');

        // Ищем, какому кораблю принадлежит эта клетка
        for (const Ship& ship : _ships) {
            bool is_part = false;
            for (int i = 0; i < ship.size(); i++) {
                int r = ship.row() + (ship.direction() == Vertical ? i : 0);
                int c = ship.col() + (ship.direction() == Horizontal ? i : 0);
                char c_char = static_cast<char>('A' + c - 1);
                if (r == row && c_char == col) {
                    is_part = true;
                    break;
                }
            }

            if (is_part) {
                // проверяем, уничтожен ли этот корабль полностью
                bool destroyed = true;
                for (int i = 0; i < ship.size(); i++) {
                    int r = ship.row() + (ship.direction() == Vertical ? i : 0);
                    int c = ship.col() + (ship.direction() == Horizontal ? i : 0);
                    char c_char = static_cast<char>('A' + c - 1);

                   
                    if (_gamefield.get(r, c_char) != 'X') {
                        destroyed = false;
                        break;
                    }
                }

                if (destroyed) {
                    int size = ship.size();
                    _ships_counts[size - 1]--;

                    switch (size) {
                    case 1: return BoatDestroyed;
                    case 2: return DestroyersDestroyed;
                    case 3: return CruisersDestroyed;
                    case 4: return BattleshipDestroyed;
                    default: return Hit;
                    }
                }
                else {
                    return Hit;
                }
            }
        }
    }

    return Missed;
}

void Player::show_field(bool hide_ships) const {
    std::cout << to_string(_gamefield, hide_ships) << "\n\n";

    std::cout << "Ships Left:\n";
    std::cout << "* - " << _ships_counts[0]
        << " ** - " << _ships_counts[1]
        << " *** - " << _ships_counts[2]
        << " **** - " << _ships_counts[3] << "\n";
}

bool Player::check_lose() const noexcept {
    return (_ships_counts[0] == 0 && _ships_counts[1] == 0 &&
        _ships_counts[2] == 0 && _ships_counts[3] == 0);
}

bool Player::check_ready() const noexcept {
    return (_ships_counts[0] == _max_ships_counts[0] &&
        _ships_counts[1] == _max_ships_counts[1] &&
        _ships_counts[2] == _max_ships_counts[2] &&
        _ships_counts[3] == _max_ships_counts[3]);
}




void Game::user_add_ship(const std::string& input) {
    std::istringstream iss(input);
    int size, row;
    char col, dir;

    // проверяем что ввод не пустой
    if (input.empty() || iss.fail()) {
        throw std::logic_error("Пустой ввод. Формат: размер строка колонка направление (например: 1 1 A H)");
    }

    // пытаемся считать параметры
    if (!(iss >> size >> row >> col >> dir)) {
        throw std::logic_error("Неверный формат ввода. Ожидается: размер строка колонка направление (например: 1 1 A H)");
    }

    std::string remaining;
    if (iss >> remaining) {
        throw std::logic_error("Лишние данные после корабля. Вводите только один корабль за раз");
    }

    if (size < 1 || size > 4) {
        throw std::logic_error("Недопустимый размер корабля: " + std::to_string(size) + ". Допустимые значения: 1-4");
    }

    char dir_upper = std::toupper(static_cast<unsigned char>(dir));
    if (dir_upper != 'H' && dir_upper != 'V') {
        throw std::logic_error("Недопустимое направление: '" + std::string(1, dir) + "'. Используйте 'H' (горизонтально) или 'V' (вертикально)");
    }

    if (row < 1 || row > 10) {
        throw std::logic_error("Недопустимый номер строки: " + std::to_string(row) + ". Допустимые значения: 1-10");
    }

    char col_upper = std::toupper(static_cast<unsigned char>(col));
    if (col_upper < 'A' || col_upper > 'J') {
        throw std::logic_error("Недопустимая колонка: '" + std::string(1, col) + "'. Допустимые значения: A-J");
    }

    try {
        _user.set_ship(Ship(size, dir_upper, row, col_upper));
    }
    catch (const std::logic_error& e) {
        std::string error_msg = e.what();

        // уточняем сообщение об ошибке
        if (error_msg.find("incorrect field") != std::string::npos) {
            if (error_msg.find("collision") != std::string::npos ||
                error_msg.find("кас") != std::string::npos) {
                throw std::logic_error("Корабль пересекается или касается другого корабля. Выберите другую позицию");
            }
            if (error_msg.find("out of bounds") != std::string::npos ||
                error_msg.find("границ") != std::string::npos) {
                throw std::logic_error("Корабль выходит за границы поля. Проверьте координаты и направление");
            }
            throw std::logic_error("Невозможно разместить корабль в указанной позиции");
        }

        throw; // Пробрасываем оригинальное исключение, если не распознали
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
            throw std::logic_error("Ошибка при размещении корабля #" + std::to_string(ship_count) +
                " (" + std::to_string(size) + " " + std::to_string(row) +
                " " + col + " " + dir + "): " + e.what());
        }
    }

    // Проверяем, что были введены данные
    if (ship_count == 0) {
        throw std::logic_error("Не введено ни одного корабля");
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
    _computer.show_field(true); // true = скрыть корабли

    std::cout << "\n=== YOUR PLAY FIELD ===\n\n";
    _user.show_field(false); // false = показать корабли
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