#include "Ship.h"

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
Ship::Ship(const Ship& other) : _size(other.size()), _coord(other._coord), _direction(other._direction) {}
bool Ship::is_collision(int size, const Position& coord, Direction dir) const noexcept { //True - выходит за пределы поля
    if (dir == Horizontal) {
        //Вертикальное положение корабля проверяется в конструкторе Position
        if ((coord.col() < 1) || (coord.col() + (size - 1) > coord.max_col())) {
            return true;
        }
        return false;
    }
    if (dir == Vertical) {
        //Часть корректности вертикального положения проверяется в конструкторе Position
        if ((coord.row() + (size - 1)) > coord.max_row()) {
            return true;
        }
        else if ((coord.col() < 1) || (coord.col() > coord.max_col())) {
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