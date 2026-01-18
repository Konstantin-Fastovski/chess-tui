//
// Created by felix on 1/18/26.
//

#include "chess-tui/player.hpp"

#include <unordered_map>
#include <bits/this_thread_sleep.h>

#include "chess-tui/piece-visitor.hpp"

using namespace std::chrono_literals;


Move LocalPlayer::requestMove()
{
    std::cout << "Please input move (Format a2b4): ";
    std::string input;
    std::cin >> input;
    return convertMove(input);
}

BasicBotPlayer::BasicBotPlayer(Board &board, const bool white) : board(board), white(white) {
}

Move BasicBotPlayer::requestMove() {
    std::this_thread::sleep_for(500ms);
    std::cout << "Thinking..." << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::unordered_map<std::shared_ptr<Piece>, std::set<BoardPos>> pieces;
    for (int8_t y = 0; y < 8; ++y) {
        for (int8_t x = 0; x < 8; ++x) {
            const auto & piece = board.getPiece({x, y});
            if (piece && piece->white == this->white) {
                reachable_cells_visitor visitor{this->board, {x, y}, this->white};
                piece->visit(visitor);
                if (visitor.reachable_cells.empty()) continue;
                pieces[piece] = visitor.reachable_cells;
            }
        }
    }
    std::random_device dev;
    std::mt19937_64 rng(dev());
    std::uniform_int_distribution<size_t> pieceDistribution(0, pieces.size() - 1);
    auto random_piece_iter = pieces.begin();
    std::advance(random_piece_iter, pieceDistribution(rng));
    auto [piece, cells] = *random_piece_iter;
    BoardPos from = this->board.getPos(*piece);

    std::uniform_int_distribution<size_t> cellDistribution(0, cells.size() - 1);
    auto random_cell_iter = cells.begin();
    std::advance(random_cell_iter, cellDistribution(rng));
    auto to = *random_cell_iter;

    return {from, to};
}
