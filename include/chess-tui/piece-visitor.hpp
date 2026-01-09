//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_PIECE_VISITOR_HPP
#define CHESS_TUI_PIECE_VISITOR_HPP

#include "board.hpp"
#include "chess-tui/piece.hpp"

class reachable_cells_visitor final : public PieceVisitor {
public:
    std::vector<BoardPos> reachable_cells;

    explicit reachable_cells_visitor(Board &board) : board(board) {

    }

    void visit(Pawn &pawn) override {

    }

    void visit(Rook &pawn) override {

    }

    void visit(Bishop &pawn) override {

    }

    void visit(Queen &pawn) override {

    }

    void visit(King &pawn) override {

    }

    void visit(Knight &pawn) override {

    }

private:
    Board &board;
};

#endif //CHESS_TUI_PIECE_VISITOR_HPP