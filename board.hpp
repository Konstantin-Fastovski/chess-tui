#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

struct Vector {
    int8_t x, y;
    Vector(int8_t x, int8_t y) : x(x), y(y) {}

    Vector operator*(const int& factor) {
        return Vector(x * factor, y * factor);
    }
};

typedef Vector BoardPos;

struct Piece {
    bool isWhite;
    BoardPos position;
    virtual std::vector<BoardPos> getReachableCells() = 0;
    bool isWithinGrid(Vector move) {
        return (position.x + move.x) >= 0 && (position.x + move.x) < 8 && (position.y + move.y) >= 0 && (position.y + move.y) < 8;
    }
};

struct Pawn : Piece {
    std::vector<BoardPos> getReachableCells() {
        std::vector<BoardPos> reachable_cells;
        int8_t dir = isWhite ? 1 : -1;
        uint8_t start_y = isWhite ? 1 : 6;
        if (position.y == start_y) {
            reachable_cells.push_back(Vector(position.x, position.y + 2 * dir));
        }

        Vector move = Vector(position.x, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            reachable_cells.push_back(move);
        }
        return reachable_cells;
    }
    std::vector<BoardPos> getCapturableCells() {
        std::vector<BoardPos> capturable_cells;
        int8_t dir = isWhite ? 1 : -1;
        Vector move = Vector(position.x - 1, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            capturable_cells.push_back(move);
        }
        move = Vector(position.x + 1, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            capturable_cells.push_back(move);
        }

        return capturable_cells;
    }
};

struct Rook : Piece {
    std::vector<BoardPos> getReachableCells() {
        std::vector<BoardPos> reachable_cells;
        std::vector<Vector> base_moves = {Vector(1, 0), Vector(0, -1), Vector(-1, 0), Vector(0, 1)};
        for (Vector base_move : base_moves) {
            for (int i = 1; isWithinGrid(base_move * i); i++) {
                reachable_cells.push_back(base_move * i);
            }
        }
        return reachable_cells;
    }
};

struct Move {
    Move(std::unique_ptr<Piece>& piece_ptr, BoardPos from, BoardPos to) {
    }
};

struct Board {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
};

class Player {
   public:
    virtual Move requestMove() = 0;
};