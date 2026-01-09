//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_PIECE_HPP
#define CHESS_TUI_PIECE_HPP
#include <string>
#include <vector>

#include "chess-tui/vector.hpp"

struct Pawn;
struct Rook;
struct Bishop;
struct Queen;
struct King;
struct Knight;

class PieceVisitor {
public:

  virtual void visit(Pawn &pawn) = 0;
  virtual void visit(Rook &pawn) = 0;
  virtual void visit(Bishop &pawn) = 0;
  virtual void visit(Queen &pawn) = 0;
  virtual void visit(King &pawn) = 0;
  virtual void visit(Knight &pawn) = 0;

  virtual ~PieceVisitor() = default;
};

struct Piece {
  bool white;

  explicit Piece(const bool white) : white(white) {}

  virtual std::vector<BoardPos> getReachableCells(const BoardPos &position) = 0;
  virtual std::string getUnicode() = 0;
  virtual void visit(PieceVisitor &pieceVisitor) = 0;

  virtual ~Piece() = default;
};

struct Pawn final : Piece {
  explicit Pawn(const bool white) : Piece(white) {}

  const int8_t dir = white ? 1 : -1;
  const uint8_t start_rank = white ? 1 : 6;

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;
  [[nodiscard]] std::vector<BoardPos> getCapturableCells(const BoardPos &position) const;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

struct Rook final : Piece {
  explicit Rook(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

struct Knight final : Piece {
  explicit Knight(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

struct King final : Piece {
  explicit King(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

struct Bishop final : Piece {
  explicit Bishop(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

struct Queen final : Piece {
  explicit Queen(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override;

  std::string getUnicode() override;

  void visit(PieceVisitor &pieceVisitor) override;
};

#endif //CHESS_TUI_PIECE_HPP