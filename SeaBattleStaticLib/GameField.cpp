
#include "GameField.h"
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
        if (i <= 8) { res += std::to_string(i + 1) + " |"; }
        else {
            res += std::to_string(i + 1) + "|";
        }
        for (int j = 0; j < gf._m; j++) {
            char ship_column = gf._field[i][j];

            //скрываем корабли если нужно
            if (hide_ships && ship_column == '*') {
                ship_column = ' ';
            }

            res += ship_column;

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