#pragma once
#include "Position.h"
enum Direction { Horizontal, Vertical }; // положение корабля на карте

class Ship {
private:
    int _size; //Размер корабля 1-4 ячеек
    Position _coord; //Координата хвоста корабля
    Direction _direction; //Положение: горизонтальное или вертикальное
public:
    Ship() = delete;
    Ship(int size, int row, char col) : Ship(size, 'H', row, col) {};
    Ship(int size, Position coord, Direction dir);
    Ship(int size, Position coord) : Ship(size, coord, Horizontal) {}
    Ship(int size, char dir, int row, char col);
    Ship(const Ship& other);
    bool is_collision(int size, const Position& coord, Direction dir) const noexcept;
    void rotate();

    int size() const noexcept { return _size; }
    Direction direction() const noexcept { return _direction; }
    int row() const noexcept { return _coord.row(); }
    int col() const noexcept { return _coord.col(); }
    int calc_ship_row_with_shift(int shift) const noexcept {
        return _coord.row() + (_direction == Vertical ? shift : 0);
    };
    int calc_ship_col_with_shift(int shift) const noexcept {
        return _coord.col() + (_direction == Horizontal ? shift : 0);
    };
};
