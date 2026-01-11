//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_PIECE_VISITOR_HPP
#define CHESS_TUI_PIECE_VISITOR_HPP

#include "board.hpp"
#include "chess-tui/piece.hpp"

// TODO If king is in check, return no reachable cells for all pieces

class reachable_cells_visitor final : public PieceVisitor
{
public:
    std::vector<BoardPos> reachable_cells;

    explicit reachable_cells_visitor(Board &board, const BoardPos &pos) : board(board), pos(pos)
    {
        const auto & piece = board.getPiece(pos);
        if (!piece) return;
        piece->visit(*this);
    }

    void visit(Pawn &pawn) override
    {
        const auto base_move = Vector(0, 1);

        this->check_reachable(pawn, pos + base_move * pawn.dir, true, false);
        if (pos.x == pawn.start_rank)
        {
            this->check_reachable(pawn, pos + base_move * pawn.dir * 2, true, false);
        }

        const auto base_capture = Vector(1, 1);
        this->check_reachable(pawn, base_capture * pawn.dir, false, true);
        this->check_reachable(pawn, base_capture.mirrorHorizontal() * pawn.dir, false, true);
        for (const BoardPos &reachable_cell : reachable_cells)
            printf("%i, %i  ", reachable_cell.x, reachable_cell.y);
        std::cout << std::endl;
    }

    void visit(Rook &rook) override
    {
        const auto base_move = Vector(1, 0);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            for (Vector &dest = pos;
                 this->check_reachable(rook, dest, true, true) == ReachableResult::MOVE;
                 dest += move) {
            }
        }
    }

    void visit(Knight &knight) override
    {
        const Vector base_move = Vector(2, 1);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            const BoardPos destination = pos + move;
            this->check_reachable(knight, destination, true, true);
        }
    }

    // TODO add isInCheck() method to disallow moves into check; could be computationally intensive.
    // Alternatively leave it and implement the game logic, so that the game ends when the king is captured
    void visit(King &king) override
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

        if (king.has_moved == false)
        {
            // TODO add castling
        }
    }
    void visit(Bishop &bishop) override
    {
        Vector base_move = Vector(1, 1);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            for (Vector &dest = pos;
                 this->check_reachable(bishop, dest, true, true) == ReachableResult::MOVE;
                 dest += move) {
            }
        }
    }

    void visit(Queen &queen) override
    {
        const auto base_move_diagonal = Vector(1, 1);
        for (const Vector &move : base_move_diagonal.getAllPossibleTransforms())
        {
            for (Vector &dest = pos;
                 this->check_reachable(queen, dest, true, true) == ReachableResult::MOVE;
                 dest += move) {
            }
        }
        const auto base_move_straight = Vector(1, 0);
        for (const Vector &move : base_move_straight.getAllPossibleTransforms())
        {
            for (Vector &dest = pos;
                 this->check_reachable(queen, dest, true, true) == ReachableResult::MOVE;
                 dest += move) {
            }
        }
    }

    enum class ReachableResult {
        UNREACHABLE = 0,
        MOVE = 1,
        CAPTURE = 2,
    };
    ReachableResult check_reachable(const Piece &piece, const BoardPos &dest, const bool can_walk, const bool can_capture) {
        if (!dest.isWithinGrid()) return ReachableResult::UNREACHABLE;
        const auto & target = board.getPiece(dest);
        if (!target) {
            if (!can_walk) return ReachableResult::UNREACHABLE;
            reachable_cells.push_back(dest);
            return ReachableResult::MOVE;
        }
        if (piece.white == target->white || !can_capture)
            return ReachableResult::UNREACHABLE;
        reachable_cells.push_back(dest);
        return ReachableResult::CAPTURE;
    }

private:
    Board &board;
    BoardPos pos;
};

#endif // CHESS_TUI_PIECE_VISITOR_HPP