#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Vector {
    int8_t x, y;

    Vector(const int8_t x, const int8_t y) : x(x), y(y) {}

    Vector operator*(const int& factor) const {
        return Vector(x * factor, y * factor);
    }
};

typedef Vector BoardPos;

struct Piece {
    bool white;
    BoardPos position;

    Piece(const BoardPos pos, const bool white)
        : white(white), position(pos) {
    }

    virtual std::vector<BoardPos> getReachableCells() = 0;
    virtual std::string getUnicode() = 0;

    bool isWithinGrid(const Vector move) const {
        return (position.x + move.x) >= 0 && (position.x + move.x) < 8 && (position.y + move.y) >= 0 && (position.y + move.y) < 8;
    }

    virtual ~Piece() = default;
};

struct Pawn final : Piece {
    Pawn(const BoardPos &pos, const bool white)
        : Piece(pos, white) {
    }

    std::vector<BoardPos> getReachableCells() override {
        std::vector<BoardPos> reachable_cells;
        const int8_t dir = white ? 1 : -1;
        const uint8_t start_y = white ? 1 : 6;
        if (position.y == start_y) {
            reachable_cells.push_back(Vector(position.x, position.y + 2 * dir));
        }

        const auto move = Vector(position.x, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            reachable_cells.push_back(move);
        }
        return reachable_cells;
    }
    std::vector<BoardPos> getCapturableCells() const {
        std::vector<BoardPos> capturable_cells;
        const int8_t dir = white ? 1 : -1;
        auto move = Vector(position.x - 1, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            capturable_cells.push_back(move);
        }
        move = Vector(position.x + 1, position.y + 1 * dir);
        if (isWithinGrid(move)) {
            capturable_cells.push_back(move);
        }
        return capturable_cells;
    }

    std::string getUnicode() override {
        return "\u265F";
    }
};

struct Rook final : Piece {
    Rook(const BoardPos &pos, const bool white)
        : Piece(pos, white) {
    }

    std::vector<BoardPos> getReachableCells() override {
        std::vector<BoardPos> reachable_cells;
        const std::vector<Vector> base_moves = {Vector(1, 0), Vector(0, -1), Vector(-1, 0), Vector(0, 1)};
        for (Vector base_move : base_moves) {
            for (int i = 1; isWithinGrid(base_move * i); i++) {
                reachable_cells.push_back(base_move * i);
            }
        }
        return reachable_cells;
    }

    std::string getUnicode() override {
        return "\u2656";
    }
};

struct Move {
    std::unique_ptr<Piece>& piece_ptr;
    BoardPos from;
    BoardPos to;

    Move(std::unique_ptr<Piece>& piece_ptr, const BoardPos from, const BoardPos to)
        : piece_ptr(piece_ptr), from(from), to(to) {
    }
};

struct Board {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;

    Board(){

    }

    void doMove(const Move move) {
      grid[move.to.x][move.to.y] = std::move(grid[move.from.x][move.from.y]);
    }
};

class Player {
   public:
    virtual Move requestMove() = 0;

    virtual ~Player() = default;
};

inline void drawBoard(const Board &board) {
    std::cout << "┏━━━━━━━━━━━━━━━━━┓" << std::endl;
    for (const auto& row : board.grid) {
        std::cout << "┃";
        for (const auto& piece : row) {
            if (piece) {
                std::cout << " " << piece->getUnicode();
            } else {
                std::cout << "   ";
            }
        }
        std::cout << " ┃" << std::endl;
    }
    std::cout << "┗━━━━━━━━━━━━━━━━━┛" << std::endl;
}