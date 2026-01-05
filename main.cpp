#include "board.hpp"

int main() {

    const std::array<std::unique_ptr<Player>, 2> players = {
        std::make_unique<LocalPlayer>(),
        std::make_unique<LocalPlayer>()
    };
    uint8_t current_player = 0;

    Board board;

    while (true) {
        board.draw();

        std::cout << "It's Player " << current_player+1 << "'s turn! " << std::endl;
        const auto move = players[current_player]->requestMove();
        std::cout << "Requested Move: (" << std::to_string(move.from.x) << ", " << std::to_string(move.from.y) << ")"
            << " > (" << std::to_string(move.to.x) << ", " << std::to_string(move.to.y) << ")" << std::endl;

        const auto &piece = board.getPiece(move.from);
        if (!piece) {
            std::cout << "This square is empty" << std::endl;
            continue;
        }
        if (piece->white != current_player) {
            std::cout << "This is not your piece" << std::endl;
        }
        const std::vector<BoardPos> reachable_cells = piece->getReachableCells(move.from);
        if (std::ranges::find(reachable_cells, move.to) == reachable_cells.end()) {
            std::cout << "This is not a valid move" << std::endl;
            continue;
        }
        auto &capturePiece = board.getPiece(move.to);
        if (capturePiece) {
            if (capturePiece->white == piece->white) {
                std::cout << "You cannot capture your own piece" << std::endl;
                continue;
            }
            std::cout << "You captured a " << capturePiece->getUnicode() << std::endl;
        }
        capturePiece.reset();

        board.applyMove(move);

        current_player = (current_player + 1) % 2;
    }

    return 0;
}