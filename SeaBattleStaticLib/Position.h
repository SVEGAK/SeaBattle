#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>



class Position {
private:
    int _row;
    int _col;

    static const int _max_row;
    static const int _max_col;

public:
    Position() : _row(1), _col(1) {}
    Position(const Position& other) = default;
    Position(int row, int col);
    Position(const std::string& str);

    int row() const noexcept { return _row; }
    int col() const noexcept { return _col; }
    static Position parse(const std::string& str);
    void row(int new_row);
    void col(int new_col);
    int max_row() const noexcept { return _max_row; }
    int max_col() const noexcept { return _max_col; }
    friend std::string to_string(const Position& pos);
    
};
const int Position::_max_row = 10;
const int Position::_max_col = 10;
