#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include <array>

struct Vector {
    uint8_t x, y;
};

typedef Vector BoardPos;

struct Piece {
    BoardPos position;
    virtual std::vector<BoardPos> getNextPositions() = 0;
};

struct Move {
    Move(std::unique_ptr<Piece> &piece_ptr, BoardPos from, BoardPos to) {

    }

};

struct Board {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
};

class Player {
public:
    virtual Move requestMove() = 0;
};