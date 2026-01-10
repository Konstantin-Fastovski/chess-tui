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

    explicit reachable_cells_visitor(Board &board) : board(board)
    {
    }

    void visit(Pawn &pawn) override
    {
        BoardPos pos = board.getPos(pawn);
        const auto base_move = Vector(0, 1);

        BoardPos destination = pos + base_move * pawn.dir;
        if (board.getPiece(destination) == nullptr)
            reachable_cells.push_back(
                pos + base_move * pawn.dir);

        destination = pos + base_move * pawn.dir * 2;
        if (pos.x == pawn.start_rank && board.getPiece(destination) == nullptr)
        {
            reachable_cells.push_back(destination);
        }

        const auto base_capture = Vector(1, 1);
        for (const Vector &capture : {base_capture * pawn.dir, base_capture.mirrorHorizontal() * pawn.dir})
        {
            if (board.getPiece(pos + capture) != nullptr && board.getPiece(pos + capture)->white != pawn.white)
            {
                reachable_cells.push_back(pos + capture);
            }
        }
        for (BoardPos i : reachable_cells)
            printf("%i, %i  ", i.x, i.y);
        std::cout << std::endl;
    }

    void visit(Rook &rook) override
    {
        BoardPos pos = board.getPos(rook);
        const auto base_move = Vector(1, 0);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            for (int8_t i = 1;; i++)
            {
                BoardPos destination = pos + move * i;

                if (!destination.isWithinGrid() || board.getPiece(destination) != nullptr && board.getPiece(destination)->white == rook.white)
                    break;
                reachable_cells.push_back(destination);

                if (board.getPiece(destination) != nullptr && board.getPiece(destination)->white != rook.white)
                    break;
            }
        }
    }

    void visit(Knight &knight) override
    {
        BoardPos pos = board.getPos(knight);
        const Vector base_move = Vector(2, 1);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            const BoardPos destination = pos + move;
            if (destination.isWithinGrid())
            {
                reachable_cells.push_back(destination);
            }
        }
    }

    // TODO add isInCheck() method to disallow moves into check; could be computationally intensive.
    // Alternatively leave it and implement the game logic, so that the game ends when the king is captured
    void visit(King &king) override
    {
        BoardPos pos = board.getPos(king);
        for (int8_t x = -1; x < 1; ++x)
        {
            for (int8_t y = -1; y < 1; ++y)
            {
                BoardPos destination = pos + Vector(x, y);
                if (x != 0 && y != 0 && destination.isWithinGrid())
                {
                    reachable_cells.emplace_back(destination);
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
        BoardPos pos = board.getPos(bishop);
        Vector base_move = Vector(1, 1);
        for (const Vector &move : base_move.getAllPossibleTransforms())
        {
            for (int i = 1;; i++)
            {
                BoardPos destination = pos + move * i;

                if (!destination.isWithinGrid() || board.getPiece(destination) != nullptr && board.getPiece(destination)->white == bishop.white)
                    break;
                reachable_cells.push_back(destination);

                if (board.getPiece(destination) != nullptr && board.getPiece(destination)->white != bishop.white)
                    break;
            }
        }
    }

    void visit(Queen &queen) override
    {
        BoardPos pos = board.getPos(queen);
        const auto base_move_diagonal = Vector(1, 1);
        for (const Vector &move : base_move_diagonal.getAllPossibleTransforms())
        {
            for (int8_t i = 1;; i++)
            {
                BoardPos destination = pos + move * i;

                if (!destination.isWithinGrid() || board.getPiece(destination) != nullptr && board.getPiece(destination)->white == queen.white)
                    break;
                reachable_cells.push_back(destination);

                if (board.getPiece(destination) != nullptr && board.getPiece(destination)->white != queen.white)
                    break;
            }
        }
        const auto base_move_straight = Vector(1, 0);
        for (const Vector &move : base_move_straight.getAllPossibleTransforms())
        {
            for (int8_t i = 1;; i++)
            {
                BoardPos destination = pos + move * i;

                if (!destination.isWithinGrid() || board.getPiece(destination) != nullptr && board.getPiece(destination)->white == queen.white)
                    break;
                reachable_cells.push_back(destination);

                if (board.getPiece(destination) != nullptr && board.getPiece(destination)->white != queen.white)
                    break;
            }
        }
    }

private:
    Board &board;
};

#endif // CHESS_TUI_PIECE_VISITOR_HPP