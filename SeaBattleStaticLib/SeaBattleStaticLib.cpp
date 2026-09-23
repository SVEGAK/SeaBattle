// SeaBattleStaticLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include "main.h"


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
void GameField::allocate_and_fill(char fill_char = ' ') {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = fill_char;
        }
    }
}

void GameField::copy_from(const GameField& other) {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = other._field[i][j];
        }
    }
}
GameField::GameField(char** field, int n, int m) : _n(n), _m(m) {
    check_dimensions(n, m);
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j) {
            _field[i][j] = field[i][j];
        }
    }
}

std::string to_string(const GameField& gf) {
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

    // Верхняя граница: "  +-------+"
    res += "  +";
    res += std::string(gf._m * 2 - 1, '-');
    res += "+\n";

    // Строки поля
    for (int i = 0; i < gf._n; ++i) {
        res += std::to_string(i + 1) + " |";
        for (int j = 0; j < gf._m; ++j) {
            res += gf._field[i][j];
            if (j < gf._m - 1) {
                res += " ";
            }
        }
        res += "|\n";
    }

    // Нижняя граница (без завершающего \n, как в строгом формате)
    res += "  +";
    res += std::string(gf._m * 2 - 1, '-');
    res += "+";

    return res;
}