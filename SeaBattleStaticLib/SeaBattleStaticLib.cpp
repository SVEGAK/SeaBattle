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
