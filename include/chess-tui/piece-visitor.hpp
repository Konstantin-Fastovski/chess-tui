//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_PIECE_VISITOR_HPP
#define CHESS_TUI_PIECE_VISITOR_HPP

#include "board.hpp"
#include "chess-tui/piece.hpp"

class reachable_cells_visitor;

bool is_reachable(Board &board, const BoardPos &pos, bool white);
void remove_enemy_reachable_cells(Board &board, bool white, std::set<BoardPos> &cells);

class reachable_cells_visitor final : public PieceVisitor
{
public:
    std::set<BoardPos> reachable_cells;

    explicit reachable_cells_visitor(Board &board, const BoardPos &pos, bool current_player_white);

    void visit(Pawn &pawn) override;

    void visit(Rook &rook) override;

    void visit(Knight &knight) override;

    void visit(King &king) override;

    void visit(Bishop &bishop) override;

    void visit(Queen &queen) override;

    enum class ReachableResult {
        UNREACHABLE = 0,
        MOVE = 1,
        CAPTURE = 2,
    };
    ReachableResult check_reachable(const Piece &piece, const BoardPos &dest, bool can_walk, bool can_capture);

private:
    Board &board;
    BoardPos pos;
    bool current_player_white;
};

#endif // CHESS_TUI_PIECE_VISITOR_HPP