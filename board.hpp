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

    Vector operator+(const Vector& summand) const {
        return {static_cast<int8_t>(x + summand.x), static_cast<int8_t>(y + summand.y)};
    }

    Vector mirrorHorizontal() const {
        return Vector(static_cast<int8_t>(-x), y);
    }

    Vector mirrorVertical() const {
        return Vector(x, static_cast<int8_t>(-y));
    }

    Vector rotate90(bool clockwise) const {
        return Vector(clockwise ? y : static_cast<int8_t>(-y), clockwise ? static_cast<int8_t>(-x) : x);
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

    bool isWithinGrid(const Vector& move) const {
        return (position.x + move.x) >= 0 && (position.x + move.x) < 8 && (position.y + move.y) >= 0 && (position.y + move.y) < 8;
    }

    virtual ~Piece() = default;
};

struct Pawn final : Piece {
    Pawn(const BoardPos &pos, const bool white)
        : Piece(pos, white) {
    }

    const int8_t dir = white ? 1 : -1;
    const uint8_t start_y = white ? 1 : 6;

    std::vector<BoardPos> getReachableCells() override {
        std::vector<BoardPos> reachable_cells;

        const Vector base_move = Vector(0, 1);

        if (position.y == start_y) {
            reachable_cells.emplace_back(position + base_move * 2 * dir); //why emplace_back?
        }
        
        reachable_cells.push_back(position + base_move * dir); //withinBoard checks unnecessary cuz pawns can't be on the last rank lol
        return reachable_cells;
    }
    std::vector<BoardPos> getCapturableCells() const {
        std::vector<BoardPos> capturable_cells;
        const int8_t dir = white ? 1 : -1;
        auto destination = BoardPos(position.x - 1, position.y + 1 * dir);
        if (isWithinGrid(destination)) {
            capturable_cells.push_back(destination);
        }
        destination = BoardPos(position.x + 1, position.y + 1 * dir);
        if (isWithinGrid(destination)) {
            capturable_cells.push_back(destination);
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
        const Vector base_move = Vector(1, 0);
        for (const Vector& rotated : {base_move, base_move.rotate90(true)}) {
            for (int i = 1; ; i++) {
                BoardPos destination = position + rotated * i;
                if (!isWithinGrid(destination)) break;
                reachable_cells.push_back(destination);
            }
        }
        return reachable_cells;
    }

    std::string getUnicode() override {
        return "\u2656";
    }
};

struct Knight final : Piece {
    Knight(const BoardPos &pos, const bool white)
        : Piece(pos, white) {
    }

    std::vector<BoardPos> getReachableCells() override {
        std::vector<BoardPos> reachable_cells;
        const Vector base_move = Vector(2, 1);
        for (const Vector& mirrored : {base_move, base_move.mirrorHorizontal(), base_move.mirrorVertical(), base_move.mirrorHorizontal().mirrorVertical()}) {
            for (const Vector& rotated : {mirrored, mirrored.rotate90(true)}) {
                const BoardPos destination = position + mirrored;
                if (isWithinGrid(destination)) {
                    reachable_cells.push_back(destination);
                }
            }
        }
        return reachable_cells;
    }

    std::string getUnicode() override {
        return "";
    }
};

struct Bishop final : Piece {
    Bishop(const BoardPos &pos, const bool white)
        : Piece(pos, white) {
    }

    std::vector<BoardPos> getReachableCells() override {
        std::vector<BoardPos> reachable_cells;

        Vector base_move = Vector(1, 1);
        for (const Vector& mirrored : {base_move, base_move.mirrorHorizontal(), base_move.mirrorVertical(), base_move.mirrorHorizontal().mirrorVertical()}) {
            for (int i = 1; ; i++) {
                BoardPos destination = position + mirrored * i;
                if (!isWithinGrid(destination)) break;
                reachable_cells.push_back(destination);
            }
        }
        return reachable_cells;
    }

    std::string getUnicode() override {
        return "";
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