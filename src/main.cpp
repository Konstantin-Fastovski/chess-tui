#include <fstream>

#include "chess-tui/board.hpp"
#include "chess-tui/piece.hpp"
#include "chess-tui/vector.hpp"
#include "chess-tui/piece-visitor.hpp"
#include "chess-tui/player.hpp"

void selectGamemode(Board &board, std::unique_ptr<Player> &player) {
    while (true) {
        std::cout << "Select Gamemode:" << std::endl;
        std::cout << "[1] Player vs Player" << std::endl;
        std::cout << "[2] Player vs Bot" << std::endl;
        std::string input;
        std::cin >> input;

        if (input == "1") {
            player = std::make_unique<LocalPlayer>();
            break;
        }
        if (input == "2") {
            player = std::make_unique<BasicBotPlayer>(board, false);
            break;
        }
        std::cout << "Please enter either 0 or 1" << std::endl;
    }
}

/**
 * [1] x
 * [1] y
 * [1] white
 * [1] has_moved
 * [1] type
 */
void saveGame(Board &board, std::ofstream &fout) {
    std::vector<std::shared_ptr<Piece>> pieces;
    for (int8_t y = 0; y < 8; ++y) {
        for (int8_t x = 0; x < 8; ++x) {
            const auto & piece = board.getPiece({x, y});
            if (!piece) continue;
            pieces.push_back(piece);
        }
    }
    uint8_t piece_count = pieces.size();
    fout.write(reinterpret_cast<char *>(&piece_count), sizeof(piece_count));
    for (const auto & piece : pieces) {
        auto [x, y] = board.getPos(*piece);
        fout.write(reinterpret_cast<char *>(&x), sizeof(x));
        fout.write(reinterpret_cast<char *>(&y), sizeof(x));
        uint8_t white = piece->white;
        fout.write(reinterpret_cast<char *>(&white), sizeof(white));
        uint8_t has_moved = piece->white;
        fout.write(reinterpret_cast<char *>(&has_moved), sizeof(has_moved));
        char symbol = piece->getSymbol();
        fout.write(&symbol, sizeof(symbol));
    }
}

void loadGame(Board &board, std::ifstream &fin) {
    for (int8_t y = 0; y < 8; ++y) {
        for (int8_t x = 0; x < 8; ++x) {
            board.getPiece({x, y}).reset();
        }
    }
    uint8_t piece_count;
    fin.read(reinterpret_cast<char *>(&piece_count), sizeof(piece_count));
    for (int i = 0; i < piece_count; ++i) {
        int8_t x, y;
        fin.read(reinterpret_cast<char *>(&x), sizeof(x));
        fin.read(reinterpret_cast<char *>(&y), sizeof(y));
        uint8_t white;
        fin.read(reinterpret_cast<char *>(&white), sizeof(white));
        uint8_t has_moved;
        fin.read(reinterpret_cast<char *>(&has_moved), sizeof(has_moved));
        char symbol;
        fin.read(&symbol, sizeof(symbol));
        switch (symbol) {
            case 'P':
                board.setPiece({x, y}, std::make_shared<Pawn>(white, has_moved));
                break;
            case 'R':
                board.setPiece({x, y}, std::make_shared<Rook>(white, has_moved));
                break;
            case 'B':
                board.setPiece({x, y}, std::make_shared<Bishop>(white, has_moved));
                break;
            case 'N':
                board.setPiece({x, y}, std::make_shared<Knight>(white, has_moved));
                break;
            case 'K':
                board.setPiece({x, y}, std::make_shared<King>(white, has_moved));
                break;
            case 'Q':
                board.setPiece({x, y}, std::make_shared<Queen>(white, has_moved));
                break;
            default: throw std::invalid_argument("loaded invalid piece symbol");
        }
    }
}

void saveGame(Board &board) {
    std::ofstream fout;
    fout.open("chess.data", std::ios::binary | std::ios::out);
    saveGame(board, fout);
    fout.close();
}
void loadGame(Board &board) {
    std::ifstream fin;
    fin.open("chess.data", std::ios::binary | std::ios::in);
    loadGame(board, fin);
    fin.close();
}

int main() {
    Board board;

    std::array<std::unique_ptr<Player>, 2> players = {};
    players[1] = std::make_unique<LocalPlayer>();
    selectGamemode(board, players[0]);

    bool current_player_white = true;

    while (true) {
        board.draw({});

        std::cout << "It's Player " << static_cast<uint8_t>(!current_player_white) + 1 << "'s turn! "
                << std::endl;

        bool check = false;
        King &king = board.getKing(current_player_white);
        BoardPos kingPos = board.getPos(king);
        if (is_reachable(board, kingPos, !current_player_white)) {
            check = true;
            std::cout << "Check! You can only move your King." << std::endl;
        }

        while (true) {
            Move move;
            try {
                move = players[current_player_white]->requestMove();
            } catch (std::invalid_argument &e) {
                std::cout << "Invalid Move" << std::endl;
            }

            if (move.store_game) {
                saveGame(board);
                std::cout << "Saved Game." << std::endl;
                continue;
            }
            if (move.load_game) {
                loadGame(board);
                std::cout << "Loaded Game." << std::endl;
                board.draw({});
                continue;
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

                const bool is_short_castle = move.castling == 1;
                Rook &rook = board.getInitialRook(current_player_white, is_short_castle);

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
                    king.has_moved = true;
                    rook.has_moved = true;
                    break;
                }

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
                king.has_moved = true;
                rook.has_moved = true;
                break;
            }

            const auto &piece = board.getPiece(move.from);
            if (!piece) {
                std::cout << "This square is empty" << std::endl;
                continue;
            }
            if (piece->white != current_player_white) {
                std::cout << "This is not your piece" << std::endl;
                continue;
            }
            if (check && piece.get() != &board.getKing(current_player_white)) {
                std::cout << "You can only move your King" << std::endl;
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
            if (capturePiece.get() == &board.getKing(!current_player_white)) {
                std::cout << "Player " << static_cast<uint8_t>(!current_player_white) + 1 << " Won!" << std::endl;
                return EXIT_SUCCESS;
            }
            capturePiece.reset();

            board.movePiece(move.from, move.to);
            board.getPiece(move.to)->has_moved = true;
            break;
        }

        current_player_white = !current_player_white;
    }

    return 0;
}
