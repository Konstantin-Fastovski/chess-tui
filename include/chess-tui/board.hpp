#pragma once
#include <array>
#include <ranges>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "chess-tui/vector.hpp"
#include "chess-tui/piece.hpp"

inline BoardPos parseBoardPos(const std::string &input);

struct Move
{
  BoardPos from;
  BoardPos to;

  Move(const BoardPos from, const BoardPos to);
};

struct Board
{
  std::array<std::shared_ptr<King>, 2> kings;
  std::array<std::shared_ptr<Rook>, 4> rooks;
  std::array<std::array<std::shared_ptr<Piece>, 8>, 8> grid;

  Board();

  void applyMove(Move move);

  void draw() const;

  std::shared_ptr<Piece> &getPiece(const BoardPos &pos);

  [[nodiscard]] BoardPos getPos(const Piece &piece) const;

  [[nodiscard]] King &getKing(bool white) const;
};

class Player
{
public:
  virtual Move requestMove() = 0;

  virtual ~Player() = default;
};

/**
 * Currently only supports format (a2b3)
 */
const char pieces[] = {'B', 'K', 'N', 'Q', 'R'};

Move convertMove(const std::string &input);


class LocalPlayer final : public Player
{
public:
  Move requestMove() override;
};
