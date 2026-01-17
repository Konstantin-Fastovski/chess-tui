#include "chess-tui/board.hpp"
#include "chess-tui/piece.hpp"
#include "chess-tui/vector.hpp"
#include "chess-tui/piece-visitor.hpp"

int main() {
    const std::array<std::unique_ptr<Player>, 2> players = {
        std::make_unique<LocalPlayer>(), std::make_unique<LocalPlayer>()
    };
    bool current_player_white = true;

    Board board;

    while (true) {
        board.draw({});

        std::cout << "It's Player " << static_cast<uint8_t>(!current_player_white) + 1 << "'s turn! "
                << std::endl;
        Move move;
        try {
            move = players[current_player_white]->requestMove();
        } catch (std::invalid_argument &e) {
            std::cout << "Invalid Move" << std::endl;
            continue;
        }
        if (move.castling != 0) {
            std::cout << "Requested Castling";
        } else {
            std::cout << "Requested Move: (" << std::to_string(move.from.x) << ", "
                    << std::to_string(move.from.y) << ")"
                    << " > (" << std::to_string(move.to.x) << ", "
                    << std::to_string(move.to.y) << ")" << std::endl;
        }

        bool check = false;
        King &king = board.getKing(current_player_white);
        if (is_reachable(board, board.getPos(king), !current_player_white)) {
            check = true;
        }

        if (move.castling) {
            if (check) {
                std::cout << "Cannot castle out of check";
                continue;
            }

            const King &king = board.getKing(current_player_white);

            if (king.has_moved) {
                std::cout << "Cannot castle: King already moved";
                continue;
            }

            const bool is_long_castle = move.castling == 2;
            const Rook &rook = board.getInitialRook(current_player_white, is_long_castle);

            const int firstrook = king.white ? 2 : 0;
            int8_t rank = king.white ? 0 : 7;
            if (!board.initial_rooks[firstrook]->has_moved && !board.getPiece({1, rank}) && !board.getPiece({2, rank})) {
                if (!is_reachable(board, {1, rank}, !king.white) && !is_reachable(board, {2, rank}, !king.white)) {
                    this->reachable_cells.emplace(2, rank);
                }
            } else if (!board.initial_rooks[firstrook + 1]->has_moved && !board.getPiece({4, rank}) && !board.getPiece({5, rank}))) {
                if (!is_reachable(board, {4, rank}, !king.white) && !is_reachable(board, {5, rank}, !king.white)) {
                    this->reachable_cells.emplace(5, rank);
                }
            }
        } else {
            const auto &piece = board.getPiece(move.from);
            if (!piece) {
                std::cout << "This square is empty" << std::endl;
                continue;
            }
            if (piece->white != current_player_white) {
                std::cout << "This is not your piece" << std::endl;
                continue;
            }
            auto visitor = reachable_cells_visitor(board, move.from, current_player_white);

            if (std::ranges::find(visitor.reachable_cells, move.to) == visitor.reachable_cells.end()) {
                std::cout << "This is not a valid move" << std::endl;
                continue;
            }
            auto &capturePiece = board.getPiece(move.to);
            if (capturePiece) {
                std::cout << "You captured a " << capturePiece->getUnicode() << std::endl;
            }
            capturePiece.reset();
            if (dynamic_cast<King*>(capturePiece.get())) {
                std::cout << "Player " << static_cast<uint8_t>(current_player_white) + 1 << " Won!";
                return EXIT_SUCCESS;
            }

            board.movePiece(move.from, move.to);
        }

        current_player_white = !current_player_white;
    }

    return 0;
}
