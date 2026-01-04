#include "board.hpp"

int main() {

    std::array<std::unique_ptr<Player>, 2> players = {
        std::make_unique<LocalPlayer>(),
        std::make_unique<LocalPlayer>()
    };
    uint8_t current_player = 0;

    Board board;

    while (!board.hasPlayerWon()) {
        std::cout << "It's Player " << current_player+1 << "'s turn! " << std::endl;
        auto move = players[current_player]->requestMove();

        // TODO: Validate move
        // TODO: Update board

        drawBoard(board);
        current_player = (current_player + 1) % 2;
    }

    return 0;
}