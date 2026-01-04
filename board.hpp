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

    Vector operator*(const int8_t& factor) const {
        return {static_cast<int8_t>(x * factor), static_cast<int8_t>(y * factor)};
    }
};

typedef Vector BoardPos;

inline BoardPos parseBoardPos(const std::string &input) {
    if (input.size() != 2) {
        throw std::invalid_argument("invalid square input");
    }
    const uint8_t file = input[0];
    const uint8_t rank = input[1];
    if (file < 97 || file > 104) { // Some Unicode magic numbers
        throw std::invalid_argument("invalid file symbol");
    }
    if (rank < 48 || rank > 56) { // Some Unicode magic numbers
        throw std::invalid_argument("invalid rank symbol");
    }
    return {static_cast<uint8_t>(rank-48), static_cast<uint8_t>(file-97)};
}

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
            reachable_cells.emplace_back(position.x, position.y + 2 * dir);
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
    BoardPos from;
    BoardPos to;

    Move(const BoardPos from, const BoardPos to)
        : from(from), to(to) {
    }
};

struct Board {
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;

    // TODO: This method should return some kind of "MoveResult" which also states if a player won and so on
    void applyMove(const Move move) {
      grid[move.to.x][move.to.y] = std::move(grid[move.from.x][move.from.y]);
    }

    void draw() const {
        std::cout << "┏━━━━━━━━━━━━━━━━━┓" << std::endl;
        for (const auto& row : this->grid) {
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
};

class Player {
   public:
    virtual Move requestMove() = 0;

    virtual ~Player() = default;
};

/**
 * Currently only supports format (a2b3)
 */
inline Move convertMove(const std::string& input) {
    if (input.size() != 4) {
        throw std::invalid_argument("invalid move input");
    }
    BoardPos fromPos = parseBoardPos(input.substr(0, 2));
    BoardPos toPos = parseBoardPos(input.substr(2, 4));
    return {fromPos, toPos};
}

class LocalPlayer final : public Player {
public:
    Move requestMove() override {
        std::cout << "Please input move (Format a2b4): ";
        std::string input;
        std::cin >> input;
        return convertMove(input);
    }
};