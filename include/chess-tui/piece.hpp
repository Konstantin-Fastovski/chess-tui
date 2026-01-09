//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_PIECE_HPP
#define CHESS_TUI_PIECE_HPP
#include <string>
#include <vector>

#include "chess-tui/vector.hpp"


struct Piece {
  bool white;

  explicit Piece(const bool white) : white(white) {}

  virtual std::vector<BoardPos> getReachableCells(const BoardPos &position) = 0;
  virtual std::string getUnicode() = 0;

  virtual ~Piece() = default;
};

struct Pawn final : Piece {
  explicit Pawn(const bool white) : Piece(white) {}

  const int8_t dir = white ? 1 : -1;
  const uint8_t start_x = white ? 1 : 6;

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;
  [[nodiscard]] std::vector<BoardPos> getCapturableCells(const BoardPos &position) const;

  std::string getUnicode() override;
};

struct Rook final : Piece {
  explicit Rook(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;
};

struct Knight final : Piece {
  explicit Knight(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;
};

struct King final : Piece {
  explicit King(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;
};

struct Bishop final : Piece {
  explicit Bishop(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;
};

struct Queen final : Piece {
  explicit Queen(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;
};

#endif //CHESS_TUI_PIECE_HPP