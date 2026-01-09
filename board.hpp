#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "chess-tui/vector.hpp"
#include "chess-tui/piece.hpp"


inline BoardPos parseBoardPos(const std::string &input) {
  if (input.size() != 2) {
    throw std::invalid_argument("invalid square input");
  }
  const uint8_t file = input[0];
  const uint8_t rank = input[1];
  if (file < 97 || file > 104) { // Some Unicode magic numbers
    throw std::invalid_argument("invalid file symbol");
  }
  if (rank < 48 || rank > 56) { // Some Unicode magic numbers
    throw std::invalid_argument("invalid rank symbol");
  }
  return {static_cast<int8_t>(8 - (rank - 48)), static_cast<int8_t>(file - 97)};
}


struct Move {
  BoardPos from;
  BoardPos to;

  Move(const BoardPos from, const BoardPos to) : from(from), to(to) {}
};

struct Board {
  std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;

  Board() {
    for (int y = 0; y < 8; ++y) {
      this->grid[1][y] = std::make_unique<Pawn>(true);
      this->grid[6][y] = std::make_unique<Pawn>(false);
    }
    this->grid[0][0] = std::make_unique<Rook>(true);
    this->grid[0][7] = std::make_unique<Rook>(true);
    this->grid[7][0] = std::make_unique<Rook>(false);
    this->grid[7][7] = std::make_unique<Rook>(false);
    this->grid[0][1] = std::make_unique<Knight>(true);
    this->grid[0][6] = std::make_unique<Knight>(true);
    this->grid[7][1] = std::make_unique<Knight>(false);
    this->grid[7][6] = std::make_unique<Knight>(false);
    this->grid[0][2] = std::make_unique<Bishop>(true);
    this->grid[0][5] = std::make_unique<Bishop>(true);
    this->grid[7][2] = std::make_unique<Bishop>(false);
    this->grid[7][5] = std::make_unique<Bishop>(false);
    this->grid[0][3] = std::make_unique<Queen>(true);
    this->grid[0][4] = std::make_unique<King>(true);
    this->grid[7][3] = std::make_unique<King>(false);
    this->grid[7][4] = std::make_unique<Queen>(false);
  }

  // TODO: This method should return some kind of "MoveResult" which also states
  // if a player won and so on
  void applyMove(const Move move) {
    grid[move.to.x][move.to.y] = std::move(grid[move.from.x][move.from.y]);
  }

  void draw() const {
    std::cout << "┏━━━━━━━━━━━━━━━━━━━┓" << std::endl;
    std::cout << "┃  a b c d e f g h  ┃" << std::endl;
    for (int x = 0; x < 8; ++x) {
      std::cout << "┃" << std::to_string(8 - x);
      for (int y = 0; y < 8; ++y) {
        if (const auto &piece = this->grid[x][y]) {
          std::cout << " " << piece->getUnicode();
        } else {
          std::cout << "  ";
        }
      }
      std::cout << " " << std::to_string(8 - x) << "┃" << std::endl;
    }
    std::cout << "┃  a b c d e f g h  ┃" << std::endl;
    std::cout << "┗━━━━━━━━━━━━━━━━━━━┛" << std::endl;
  }

  std::unique_ptr<Piece> &getPiece(const BoardPos &pos) {
    return this->grid[pos.x][pos.y];
  }
};

class Player {
public:
  virtual Move requestMove() = 0;

  virtual ~Player() = default;
};

/**
 * Currently only supports format (a2b3)
 */
const char pieces[] = {'B', 'K', 'N', 'Q', 'R'};

inline Move convertMove(const std::string &input) {
  if (input.size() > 4 || input.size() < 2) {
    throw std::invalid_argument("invalid move input");
  }

  if (std::ranges::find(pieces, input[0]) != pieces.end()) {
    // Moves with Piece: Format like Kf3, Rbb6, Q4c6
    return {fromPos, toPos};
  }

  if (input.size() == 2) {
    // Pawn moves
    BoardPos toPos = parseBoardPos(input.substr(0, 2));
    return {fromPos, toPos};
  } else if (input.size() == 4) {
    BoardPos fromPos = parseBoardPos(input.substr(0, 2));
    BoardPos toPos = parseBoardPos(input.substr(2, 4));
    return {fromPos, toPos};
  }
  throw std::invalid_argument("invalid move input");
}

class LocalPlayer final : public Player {
public:
  Move requestMove() override {
    std::cout << "Please input move (Format a2b4): ";
    std::string input;
    std::cin >> input;
    return convertMove(input);
  }
};
