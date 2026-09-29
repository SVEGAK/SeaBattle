#pragma once
#include "Player.h"
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

void Player::show_field(bool hide_ships, bool is_input_scenario) const {
    std::cout << to_string(_gamefield, hide_ships) << "\n\n";
    if (is_input_scenario) {
        std::cout << "Ships Left to input:\n";
        std::cout << "* - " << (4 - _ships_counts[0])
            << " ** - " << (3 - _ships_counts[1])
            << " *** - " << (2 - _ships_counts[2])
            << " **** - " << (1 - _ships_counts[3]) << "\n";

    }
    else {
        std::cout << "Ships Left:\n";
        std::cout << "* - " << _ships_counts[0]
            << " ** - " << _ships_counts[1]
            << " *** - " << _ships_counts[2]
            << " **** - " << _ships_counts[3] << "\n";

    }

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