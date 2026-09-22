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