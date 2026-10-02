#pragma once
#include "pch.h"
#include "Player.h"
#include <iomanip>
#include <cctype>
#include <random>

#define UNIT_TEST_GAME
class Game {
private:
    Player _user;
    Player _computer;
#ifndef UNIT_TEST_GAME
    void user_init(const std::string& input);
    void user_add_ship(const std::string& input);
    void computer_init(const std::string& input);

    State user_move(const std::string& input);
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;
#endif
public:
#ifdef UNIT_TEST_GAME
    void user_init(const std::string& input);
    void user_add_ship(const std::string& input);
    void computer_init(const std::string& input);

    State user_move(const std::string& input);
    State computer_move();

    bool is_end() const noexcept;
    void show_game_window() const;
    void show_game_window_when_input() const;
#endif
    Game() = default;
    void start();

    bool isUserReady() const noexcept { return _user.check_ready(); }
    bool isUserWinner() const noexcept { return _computer.check_lose(); }
};