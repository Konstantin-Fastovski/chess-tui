//
// Created by felix on 1/18/26.
//

#ifndef CHESS_TUI_PLAYER_HPP
#define CHESS_TUI_PLAYER_HPP
#include <random>

#include "board.hpp"

class Player
{
public:
    virtual Move requestMove() = 0;

    virtual ~Player() = default;
};

/**
 * Currently only supports format (a2b3)
 */
const char piece_symbols[] = {'B', 'K', 'N', 'Q', 'R'};

Move convertMove(const std::string &input);


class LocalPlayer final : public Player
{
public:
    Move requestMove() override;
};

class BasicBotPlayer final : public Player {
    Board &board;
    bool white;
public:
    BasicBotPlayer(Board &board, bool white);

    Move requestMove() override;
};

#endif //CHESS_TUI_PLAYER_HPP