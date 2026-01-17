//
// Created by felix on 1/17/26.
//

#include "chess-tui/piece-visitor.hpp"

void remove_enemy_reachable_cells(Board &board, const bool white, std::set<BoardPos> &cells) {
    for (int8_t x = 0; x < 8; ++x) {
        for (int8_t y = 0; y < 8; ++y) {
            BoardPos testPos = {x, y};
            const std::shared_ptr<Piece>& enemy_piece = board.getPiece(testPos);
            if (!enemy_piece || enemy_piece->white == white) continue;
            reachable_cells_visitor enemy_visitor{board, testPos, white};
            enemy_piece->visit(enemy_visitor);
            for (auto reachable_cell : enemy_visitor.reachable_cells) {
                cells.erase(reachable_cell);
            }
        }
    }
}

bool is_reachable(Board &board, const BoardPos &pos, const bool white) {
    std::set cells{pos};
    remove_enemy_reachable_cells(board, white, cells);
    return !cells.empty();
}

reachable_cells_visitor::reachable_cells_visitor(Board &board, const BoardPos &pos, const bool current_player_white)
    : board(board), pos(pos), current_player_white(current_player_white)
{
    const auto & piece = board.getPiece(pos);
    if (!piece) return;
    piece->visit(*this);
}

void reachable_cells_visitor::visit(Pawn &pawn)
{
    const auto base_move = Vector(0, 1);

    this->check_reachable(pawn, pos + base_move * pawn.dir, true, false);
    if (pos.y == pawn.start_rank)
    {
        this->check_reachable(pawn, pos + base_move * pawn.dir * 2, true, false);
    }

    const auto base_capture = Vector(1, 1);
    this->check_reachable(pawn, pos + base_capture * pawn.dir, false, true);
    this->check_reachable(pawn, pos + base_capture.mirrorHorizontal() * pawn.dir, false, true);
}

void reachable_cells_visitor::visit(Rook &rook)
{
    const auto base_move = Vector(1, 0);
    for (const Vector &move : base_move.getAllPossibleTransforms())
    {
        for (Vector dest = pos + move;
             this->check_reachable(rook, dest, true, true) == ReachableResult::MOVE;
             dest += move) {
             }
    }
}

void reachable_cells_visitor::visit(Knight &knight)
{
    const auto base_move = Vector(2, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms())
    {
        const BoardPos destination = pos + move;
        this->check_reachable(knight, destination, true, true);
    }
}

void reachable_cells_visitor::visit(King &king)
{
    for (int8_t x = -1; x < 1; ++x)
    {
        for (int8_t y = -1; y < 1; ++y)
        {
            BoardPos destination = pos + Vector(x, y);
            if (x != 0 && y != 0) {
                this->check_reachable(king, destination, true, true);
            }
        }
    }

    if (king.white == this->current_player_white) {
        remove_enemy_reachable_cells(this->board, king.white, this->reachable_cells);
    }
}

void reachable_cells_visitor::visit(Bishop &bishop)
{
    const auto base_move = Vector(1, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms())
    {
        for (Vector dest = pos + move;
             this->check_reachable(bishop, dest, true, true) == ReachableResult::MOVE;
             dest += move) {
             }
    }
}

void reachable_cells_visitor::visit(Queen &queen)
{
    const auto base_move_diagonal = Vector(1, 1);
    for (const Vector &move : base_move_diagonal.getAllPossibleTransforms())
    {
        for (Vector dest = pos + move;
             this->check_reachable(queen, dest, true, true) == ReachableResult::MOVE;
             dest += move) {
             }
    }
    const auto base_move_straight = Vector(1, 0);
    for (const Vector &move : base_move_straight.getAllPossibleTransforms())
    {
        for (Vector dest = pos + move;
             this->check_reachable(queen, dest, true, true) == ReachableResult::MOVE;
             dest += move) {
             }
    }
}

reachable_cells_visitor::ReachableResult reachable_cells_visitor::check_reachable(const Piece &piece,
const BoardPos &dest, const bool can_walk, const bool can_capture) {
    if (!dest.isWithinGrid()) return ReachableResult::UNREACHABLE;
    const auto &target = board.getPiece(dest);
    if (!target) {
        if (!can_walk) return ReachableResult::UNREACHABLE;
        reachable_cells.emplace(dest);
        return ReachableResult::MOVE;
    }
    if (piece.white == target->white || !can_capture)
        return ReachableResult::UNREACHABLE;
    reachable_cells.emplace(dest);
    return ReachableResult::CAPTURE;
}