//
// Created by felix on 1/9/26.
//

#ifndef CHESS_TUI_VECTOR_HPP
#define CHESS_TUI_VECTOR_HPP
#include <cstdint>
#include <set>
#include <vector>


struct Vector {
  int8_t x, y;

  Vector(int8_t x, int8_t y);

  Vector operator*(const int8_t &factor) const;
  Vector &operator*=(const int8_t &factor);
  Vector operator+(const Vector &summand) const;
  Vector &operator+=(const Vector &vector);
  Vector operator-(const Vector &summand) const;
  Vector &operator-=(const Vector &vector);

  bool operator==(const Vector &other) const;

  [[nodiscard]] Vector mirrorHorizontal() const;

  [[nodiscard]] Vector mirrorVertical() const;

  [[nodiscard]] Vector rotate90(bool clockwise) const;

  [[nodiscard]] std::set<Vector> getAllPossibleTransforms() const;

  [[nodiscard]] bool isWithinGrid() const;

  bool operator<(const Vector &other) const;
};

typedef Vector BoardPos;

#endif //CHESS_TUI_VECTOR_HPP