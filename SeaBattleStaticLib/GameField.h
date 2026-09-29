#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>

class GameField {
private:
    char** _field;
    const int _n;
    const int _m;

    void check_dimensions(int n, int m) const;


    void check_position(int row, char col) const;


    // выделение памяти и заполнение поля
    void allocate_and_fill(char fill_char = ' ');


    // глубокое копирование из другого объекта
    void copy_from(const GameField& other);


    // Освобождение памяти
    void destroy() noexcept {
        for (int i = 0; i < _n; ++i) {
            delete[] _field[i];
        }
        delete[] _field;
    }

public:

    GameField() : _n(10), _m(10) {
        allocate_and_fill(' ');
    }

    GameField(int n, int m) : _n(n), _m(m) {
        check_dimensions(n, m);
        allocate_and_fill(' ');
    }

    GameField(char** field, int n, int m);

    GameField(const GameField& other) : _n(other._n), _m(other._m) {
        copy_from(other);
    }

    ~GameField() {
        destroy();
    }


    void set(int row, char col) {
        check_position(row, col);
        _field[row - 1][col - 'A'] = '*';
    }


    char get(int row, char col) const {
        check_position(row, col);
        return _field[row - 1][col - 'A'];
    }

    void set_char(int row, char col, char value) {
        if (row < 1 || row > _n || col < 'A' || col >= 'A' + _m)
            throw std::logic_error("Invalid input: incorrect position");
        _field[row - 1][col - 'A'] = value;
    }

    friend std::string to_string(const GameField& gf, bool hide_ships);
};
