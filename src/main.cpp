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

        bool check = false;
        King &king = board.getKing(current_player_white);
        BoardPos kingPos = board.getPos(king);
        if (is_reachable(board, kingPos, !current_player_white)) {
            std::cout << "Is in Check!" << std::endl;
            check = true;

            auto visitor = reachable_cells_visitor(board, kingPos, current_player_white);
            visitor.visit(king);
            if (visitor.reachable_cells.empty()) {
                std::cout << "Player " << static_cast<uint8_t>(current_player_white) + 1 << " Won!" << std::endl;
                return EXIT_SUCCESS;
            }
        }

        Move move;
        try {
            move = players[current_player_white]->requestMove();
        } catch (std::invalid_argument &e) {
            std::cout << "Invalid Move" << std::endl;
            continue;
        }
        if (move.castling != 0) {
            std::cout << "Requested Castling" << std::endl;
        } else {
            std::cout << "Requested Move: (" << std::to_string(move.from.x) << ", "
                    << std::to_string(move.from.y) << ")"
                    << " > (" << std::to_string(move.to.x) << ", "
                    << std::to_string(move.to.y) << ")" << std::endl;
        }

        if (move.castling) {
            if (check) {
                std::cout << "Cannot castle out of check" << std::endl;
                continue;
            }

            if (king.has_moved) {
                std::cout << "Cannot castle: King already moved" << std::endl;
                continue;
            }

            const bool is_short_castle = move.castling == 2;
            const Rook &rook = board.getInitialRook(current_player_white, is_short_castle);

            if (rook.has_moved) {
                std::cout << "Cannot castle: Rook already moved" << std::endl;
                continue;
            }

            int8_t rank = king.white ? 0 : 7;

            if (is_short_castle) {
                if (board.getPiece({5, rank}) || board.getPiece({6, rank})) {
                    std::cout << "Cannot castle: Pieces in the way" << std::endl;
                    continue;
                }
                if (is_reachable(board, {5, rank}, !king.white) || is_reachable(board, {6, rank}, !king.white)) {
                    std::cout << "Cannot castle through/into check" << std::endl;
                    continue;
                }
                board.movePiece({4, rank}, {6, rank});
                board.movePiece({7, rank}, {5, rank});
            } else {
                if (board.getPiece({2, rank}) || board.getPiece({3, rank})) {
                    std::cout << "Cannot castle: Pieces in the way" << std::endl;
                    continue;
                }
                if (is_reachable(board, {2, rank}, !king.white) || is_reachable(board, {3, rank}, !king.white)) {
                    std::cout << "Cannot castle through/into check" << std::endl;
                    continue;
                }
                board.movePiece({4, rank}, {2, rank});
                board.movePiece({0, rank}, {3, rank});
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

            board.movePiece(move.from, move.to);
        }

        current_player_white = !current_player_white;
    }

    return 0;
}
