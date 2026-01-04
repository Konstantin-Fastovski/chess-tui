#include "board.hpp"

int main() {

    Board board;

    for (int8_t i = 0; i < 8; ++i) {
        for (int8_t j = 0; j < 8; ++j) {
            BoardPos pos{i, j};
            board.grid[i][j] = std::make_unique<Pawn>(pos, true);
        }
    }

    for (const auto& row : board.grid) {
        for (const auto& piece : row) {
        }
    }

    drawBoard(board);

    return 0;
}