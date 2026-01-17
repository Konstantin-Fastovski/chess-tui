#include "chess-tui/board.hpp"

BoardPos parseBoardPos(const std::string &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("invalid square input");
    }
    const uint8_t file = input[0];
    const uint8_t rank = input[1];
    if (file < 97 || file > 104)
    { // Some Unicode magic numbers
        throw std::invalid_argument("invalid file symbol");
    }
    if (rank < 48 || rank > 56)
    { // Some Unicode magic numbers
        throw std::invalid_argument("invalid rank symbol");
    }
    return {static_cast<int8_t>(file - 97), static_cast<int8_t>(rank - 48 - 1)};
}

Move::Move(const BoardPos from, const BoardPos to) : from(from),
                                                     to(to)  {}

Move::Move(const uint8_t castling) : castling(castling) {
}

Move::Move() = default;

Board::Board()
{
    for (int x = 0; x < 8; ++x)
    {
        this->grid[1][x] = std::make_shared<Pawn>(true);
        this->grid[6][x] = std::make_shared<Pawn>(false);
    }
    this->initial_rooks[0] = std::make_shared<Rook>(false);
    this->initial_rooks[1] = std::make_shared<Rook>(false);
    this->initial_rooks[2] = std::make_shared<Rook>(true);
    this->initial_rooks[3] = std::make_shared<Rook>(true);
    this->grid[7][0] = this->initial_rooks[0];
    this->grid[7][7] = this->initial_rooks[1];
    this->grid[0][0] = this->initial_rooks[2];
    this->grid[0][7] = this->initial_rooks[3];
    this->grid[0][1] = std::make_shared<Knight>(true);
    this->grid[0][6] = std::make_shared<Knight>(true);
    this->grid[7][1] = std::make_shared<Knight>(false);
    this->grid[7][6] = std::make_shared<Knight>(false);
    this->grid[0][2] = std::make_shared<Bishop>(true);
    this->grid[0][5] = std::make_shared<Bishop>(true);
    this->grid[7][2] = std::make_shared<Bishop>(false);
    this->grid[7][5] = std::make_shared<Bishop>(false);
    this->grid[0][3] = std::make_shared<Queen>(true);
    this->grid[7][3] = std::make_shared<Queen>(false);
    this->kings[0] = std::make_shared<King>(false);
    this->kings[1] = std::make_shared<King>(true);
    this->grid[7][4] = this->kings[0];
    this->grid[0][4] = this->kings[1];
}

void Board::movePiece(const BoardPos &from, const BoardPos &to)
{
    grid[to.y][to.x] = std::move(grid[from.y][from.x]);
}

void Board::draw(const std::set<BoardPos> &marked_cells) const
{
    std::cout << "┏━━━━━━━━━━━━━━━━━━━┓" << std::endl;
    std::cout << "┃  a b c d e f g h  ┃" << std::endl;
    for (int8_t y = 7; y >= 0; --y)
    {
        std::cout << "┃" << std::to_string(y+1);
        for (int8_t x = 0; x < 8; ++x)
        {
            if (marked_cells.contains({x, y})) {
                std::cout << " █";
            } else if (const auto &piece = this->grid[y][x])
            {
                std::cout << " " << piece->getUnicode();
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << " " << std::to_string(y+1) << "┃" << std::endl;
    }
    std::cout << "┃  a b c d e f g h  ┃" << std::endl;
    std::cout << "┗━━━━━━━━━━━━━━━━━━━┛" << std::endl;
}

std::shared_ptr<Piece> &Board::getPiece(const BoardPos &pos) {
    return this->grid[pos.y][pos.x];
}

BoardPos Board::getPos(const Piece &piece) const {
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            if (this->grid[y][x].get() == &piece) {
                return {x, y};
            }
        }
    }
    throw std::invalid_argument("piece not on board");
}

King & Board::getKing(const bool white) const {
    return *this->kings[static_cast<uint8_t>(white)];
}

Rook & Board::getInitialRook(const bool white, const bool short_side) const {
    return *this->initial_rooks[white * 2 + short_side];
}

Move convertMove(const std::string &input)
{
    if (input == "O-O-O") {
        return Move(2);
    }
    if (input == "O-O") {
        return Move(1);
    }
    if (input.size() > 4 || input.size() < 2)
    {
        throw std::invalid_argument("invalid move input");
    }

    /*if (std::ranges::find(pieces, input[0]) != pieces.end())
    {
        // Moves with Piece: Format like Kf3, Rbb6, Q4c6
        return {fromPos, toPos};
    }

    if (input.size() == 2)
    {
        // Pawn moves
        BoardPos toPos = parseBoardPos(input.substr(0, 2));
        return {fromPos, toPos};
    }
    else*/
    if (input.size() == 4)
    {
        BoardPos fromPos = parseBoardPos(input.substr(0, 2));
        BoardPos toPos = parseBoardPos(input.substr(2, 4));
        return {fromPos, toPos};
    }
    throw std::invalid_argument("invalid move input");
}

Move LocalPlayer::requestMove()
{
    std::cout << "Please input move (Format a2b4): ";
    std::string input;
    std::cin >> input;
    return convertMove(input);
}