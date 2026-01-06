#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Vector {
  int8_t x, y;

  Vector(const int8_t x, const int8_t y) : x(x), y(y) {}

  Vector operator*(const int8_t &factor) const {
    return {static_cast<int8_t>(x * factor), static_cast<int8_t>(y * factor)};
  }
  Vector &operator*=(const int8_t &factor) {
    this->x = static_cast<int8_t>(this->x * factor);
    this->y = static_cast<int8_t>(this->y * factor);
    return *this;
  }
  Vector operator+(const Vector &summand) const {
    return {static_cast<int8_t>(x + summand.x),
            static_cast<int8_t>(y + summand.y)};
  }
  Vector &operator+=(const Vector &vector) {
    this->x = static_cast<int8_t>(this->x + vector.x);
    this->y = static_cast<int8_t>(this->y + vector.y);
    return *this;
  }
  Vector operator-(const Vector &summand) const {
    return {static_cast<int8_t>(x + summand.x),
            static_cast<int8_t>(y + summand.y)};
  }
  Vector &operator-=(const Vector &vector) {
    this->x = static_cast<int8_t>(this->x - vector.x);
    this->y = static_cast<int8_t>(this->y - vector.y);
    return *this;
  }

  bool operator==(const Vector &other) const {
    return x == other.x && y == other.y;
  }

  Vector mirrorHorizontal() const { return Vector(static_cast<int8_t>(-x), y); }

  Vector mirrorVertical() const { return Vector(x, static_cast<int8_t>(-y)); }

  Vector rotate90(bool clockwise) const {
    return Vector(clockwise ? y : static_cast<int8_t>(-y),
                  clockwise ? static_cast<int8_t>(-x) : x);
  }

  std::vector<Vector> getAllPossibleTransforms() const {
    // Treat Vector as a shape and return all possible Vectors with the same
    // shape.

    if (x == 0) {
      return {*this, this->mirrorVertical()};
    } else if (y == 0) {
      return {*this, this->mirrorHorizontal()};
    } else if (x == y) {
      return {*this, this->mirrorHorizontal(), this->mirrorVertical(),
              this->mirrorVertical().mirrorHorizontal()};
    } else {
      std::vector<Vector> result;
      for (const Vector &mirrored :
           {*this, this->mirrorHorizontal(), this->mirrorVertical(),
            this->mirrorVertical().mirrorHorizontal()}) {
        for (const Vector &rotated : {mirrored, mirrored.rotate90(true)}) {
          result.push_back(rotated);
        }
      }
      return result;
    }
  }

  [[nodiscard]] bool isWithinGrid() const {
    return this->x >= 0 && this->x < 8 && this->y >= 0 && this->y < 8;
  }
};

typedef Vector BoardPos;

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

struct Piece {
  bool white;

  Piece(const bool white) : white(white) {}

  virtual std::vector<BoardPos> getReachableCells(const BoardPos &position) = 0;
  virtual std::string getUnicode() = 0;

  virtual ~Piece() = default;
};

struct Pawn final : Piece {
  Pawn(const bool white) : Piece(white) {}

  const int8_t dir = white ? 1 : -1;
  const uint8_t start_x = white ? 1 : 6;

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;

    const Vector base_move = Vector(1, 0);

    if (position.x == start_x) {
      reachable_cells.emplace_back(position +
                                   base_move * 2 * dir); // why emplace_back?
    }

    reachable_cells.push_back(
        position + base_move * dir); // withinBoard checks unnecessary cuz pawns
                                     // can't be on the last rank lol
    return reachable_cells;
  }
  std::vector<BoardPos> getCapturableCells(const BoardPos &position) const {
    std::vector<BoardPos> capturable_cells;
    const int8_t dir = white ? 1 : -1;
    auto destination = BoardPos(position.x - 1, position.y + 1 * dir);
    if (destination.isWithinGrid()) {
      capturable_cells.push_back(destination);
    }
    destination = BoardPos(position.x + 1, position.y + 1 * dir);
    if (destination.isWithinGrid()) {
      capturable_cells.push_back(destination);
    }
    return capturable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2659" : "\u265F";
  }
};

struct Rook final : Piece {
  Rook(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;
    const Vector base_move = Vector(1, 0);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
      for (int i = 1;; i++) {
        BoardPos destination = position + move * i;
        if (!destination.isWithinGrid())
          break;
        reachable_cells.push_back(destination);
      }
    }
    return reachable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2656" : "\u265C";
  }
};

struct Knight final : Piece {
  Knight(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;
    const Vector base_move = Vector(2, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
      const BoardPos destination = position + move;
      if (destination.isWithinGrid()) {
        reachable_cells.push_back(destination);
      }
    }
    return reachable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2658" : "\u265E";
  }
};

struct King final : Piece {
  King(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;
    for (int8_t x = -1; x < 1; ++x) {
      for (int8_t y = -1; y < 1; ++y) {
        BoardPos destination = position + Vector(x, y);
        if (x != 0 && y != 0 && destination.isWithinGrid()) {
          reachable_cells.emplace_back(destination);
        }
      }
    }
    return reachable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2654" : "\u265A";
  }
};

struct Bishop final : Piece {
  Bishop(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;

    Vector base_move = Vector(1, 1);
    for (const Vector &move : base_move.getAllPossibleTransforms()) {
      for (int i = 1;; i++) {
        BoardPos destination = position + move * i;
        if (!destination.isWithinGrid())
          break;
        reachable_cells.push_back(destination);
      }
    }
    return reachable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2657" : "\u265D";
  }
};

struct Queen final : Piece {
  explicit Queen(const bool white) : Piece(white) {}

  std::vector<BoardPos> getReachableCells(const BoardPos &position) override {
    std::vector<BoardPos> reachable_cells;

    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{1, 1}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{-1, 1}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{1, -1}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid();
         dest += Vector{-1, -1}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{1, 0}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{-1, 0}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{0, 1}) {
      reachable_cells.emplace_back(dest);
    }
    for (BoardPos dest = position; dest.isWithinGrid(); dest += Vector{0, -1}) {
      reachable_cells.emplace_back(dest);
    }

    return reachable_cells;
  }

  std::string getUnicode() override {
    return this->white ? "\u2655" : "\u265B";
  }
};

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
inline Move convertMove(const std::string &input) {
  if (input.size() != 4) {
    throw std::invalid_argument("invalid move input");
  }
  BoardPos fromPos = parseBoardPos(input.substr(0, 2));
  BoardPos toPos = parseBoardPos(input.substr(2, 4));
  return {fromPos, toPos};
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
