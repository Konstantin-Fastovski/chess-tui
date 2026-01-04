#include "board.hpp"

int main() {

    const std::array<std::unique_ptr<Player>, 2> players = {
        std::make_unique<LocalPlayer>(),
        std::make_unique<LocalPlayer>()
    };
    uint8_t current_player = 0;

    Board board;

    while (true) {
        std::cout << "It's Player " << current_player+1 << "'s turn! " << std::endl;
        const auto move = players[current_player]->requestMove();

        // TODO: Validate move
        board.applyMove(move);
        board.draw();

        current_player = (current_player + 1) % 2;
    }

    return 0;
}