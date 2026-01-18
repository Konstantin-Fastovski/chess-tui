#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "chess-tui/vector.hpp"
#include "chess-tui/piece.hpp"

inline BoardPos parseBoardPos(const std::string &input);

struct Move
{
  BoardPos from = {};
  BoardPos to = {};
  int castling = 0; // 0 is no castling, 1 is short castling, 2 is long castling
  bool store_game = false;
  bool load_game = false;

  Move(BoardPos from, BoardPos to);
  explicit Move(int castling);
  explicit Move(bool load_game);
  Move();
};

struct Board
{
  std::array<std::shared_ptr<King>, 2> kings;
  std::array<std::shared_ptr<Rook>, 4> initial_rooks;
  std::array<std::array<std::shared_ptr<Piece>, 8>, 8> grid;

  Board();
  explicit Board(const std::vector<std::shared_ptr<Piece>> &pieces);

  void movePiece(const BoardPos &from, const BoardPos &to);

  void draw(const std::set<BoardPos> &marked_cells) const;

  std::shared_ptr<Piece> &getPiece(const BoardPos &pos);

  void setPiece(const BoardPos &pos, std::shared_ptr<Piece> &&piece);

  [[nodiscard]] BoardPos getPos(const Piece &piece) const;

  [[nodiscard]] King &getKing(bool white) const;
  [[nodiscard]] Rook &getInitialRook(bool white, bool long_side) const;
};