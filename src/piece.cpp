//
// Created by felix on 1/9/26.
//

#include "chess-tui/piece.hpp"

std::vector<BoardPos> Pawn::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;

    const Vector base_move = Vector(1, 0);

    if (position.x == start_x) {
        reachable_cells.emplace_back(position +
                                     base_move * 2 * dir); // why emplace_back?
    }

    reachable_cells.push_back(
        position + base_move * dir); // withinBoard checks unnecessary cuz pawns
    // can't be on the last rank lol
    return reachable_cells;
}

std::vector<BoardPos> Pawn::getCapturableCells(const BoardPos &position) const {
    std::vector<BoardPos> capturable_cells;
    const int8_t dir = white ? 1 : -1;
    auto destination = BoardPos(position.x - 1, position.y + 1 * dir);
    if (destination.isWithinGrid()) {
        capturable_cells.push_back(destination);
    }
    destination = BoardPos(position.x + 1, position.y + 1 * dir);
    if (destination.isWithinGrid()) {
        capturable_cells.push_back(destination);
    }
    return capturable_cells;
}

std::string Pawn::getUnicode() {
    return this->white ? "\u2659" : "\u265F";
}

std::vector<BoardPos> Rook::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;
    const Vector base_move = Vector(1, 0);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
        for (int i = 1;; i++) {
            BoardPos destination = position + move * i;
            if (!destination.isWithinGrid())
                break;
            reachable_cells.push_back(destination);
        }
    }
    return reachable_cells;
}

std::string Rook::getUnicode() {
    return this->white ? "\u2656" : "\u265C";
}

std::vector<BoardPos> Knight::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;
    const Vector base_move = Vector(2, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
        const BoardPos destination = position + move;
        if (destination.isWithinGrid()) {
            reachable_cells.push_back(destination);
        }
    }
    return reachable_cells;
}

std::string Knight::getUnicode() {
    return this->white ? "\u2658" : "\u265E";
}

std::vector<BoardPos> King::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;
    for (int8_t x = -1; x < 1; ++x) {
        for (int8_t y = -1; y < 1; ++y) {
            BoardPos destination = position + Vector(x, y);
            if (x != 0 && y != 0 && destination.isWithinGrid()) {
                reachable_cells.emplace_back(destination);
            }
        }
    }
    return reachable_cells;
}

std::string King::getUnicode() {
    return this->white ? "\u2654" : "\u265A";
}

std::vector<BoardPos> Bishop::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;

    Vector base_move = Vector(1, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
        for (int i = 1;; i++) {
            BoardPos destination = position + move * i;
            if (!destination.isWithinGrid())
                break;
            reachable_cells.push_back(destination);
        }
    }
    return reachable_cells;
}

std::string Bishop::getUnicode() {
    return this->white ? "\u2657" : "\u265D";
}

std::vector<BoardPos> Queen::getReachableCells(const BoardPos &position) {
    std::vector<BoardPos> reachable_cells;

    const auto base_move_diagonal = Vector(1, 1);
    for (const Vector &move : base_move_diagonal.getAllPossibleTransforms()) {
        for (int8_t i = 1;; i++) {
            BoardPos destination = position + move * i;
            if (!destination.isWithinGrid())
                break;
            reachable_cells.push_back(destination);
        }
    }
    const auto base_move_straight = Vector(1, 0);
    for (const Vector &move : base_move_straight.getAllPossibleTransforms()) {
        for (int8_t i = 1;; i++) {
            BoardPos destination = position + move * i;
            if (!destination.isWithinGrid())
                break;
            reachable_cells.push_back(destination);
        }
    }

    return reachable_cells;
}

std::string Queen::getUnicode() {
    return this->white ? "\u2655" : "\u265B";
}