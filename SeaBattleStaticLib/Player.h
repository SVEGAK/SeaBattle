#pragma once
#include "GameField.h"
#include "Ship.h"
#include <vector>
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

    void show_field(bool hide_ships = false, bool is_input_scenario = false) const;

    bool check_lose() const noexcept;

    bool check_ready() const noexcept;
};
