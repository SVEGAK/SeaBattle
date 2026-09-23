#pragma once
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <vector>
#include <iostream>

class Position {
private:
    int _row;
    int _col;

    static const int _max_row;
    static const int _max_col;

public:
    Position() : _row(1), _col(1) {}
    Position(const Position& other) : _row(other._row), _col(other._col) {}
    Position(int row, int col);
    Position(const std::string& str);

    int row() const noexcept { return _row; }
    int col() const noexcept { return _col; }
    void row(int new_row);
    void col(int new_col);
    friend std::string to_string(const Position& pos);
    friend Position parse(const std::string& str);
    friend class Ship;
};
const int Position::_max_row = 10;
const int Position::_max_col = 10;

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
};



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



enum State { Missed, BoatDestroyed, DestroyersDestroyed, CruisersDestroyed, BattleshipDestroyed, Hit };

class Player {
private:
    GameField _gamefield;
    int _ships_counts[4];
    std::vector<Ship> _ships;

    static const int _max_ships_counts[4];

    
    bool checkCollision(const Ship& ship) const;

public:

    Player();
 
    void set_ship(const Ship& ship);

    State set_action(int row, char col);

    void show_field(bool hide_ships = false) const;

    bool check_lose() const noexcept;

    bool check_ready() const noexcept;
};