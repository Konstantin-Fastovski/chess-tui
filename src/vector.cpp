//
// Created by felix on 1/9/26.
//

#include "chess-tui/vector.hpp"

Vector::Vector(const int8_t x, const int8_t y) : x(x), y(y) {

}

Vector Vector::operator*(const int8_t &factor) const {
    return {static_cast<int8_t>(x * factor), static_cast<int8_t>(y * factor)};
}

Vector & Vector::operator*=(const int8_t &factor) {
    this->x = static_cast<int8_t>(this->x * factor);
    this->y = static_cast<int8_t>(this->y * factor);
    return *this;
}

Vector Vector::operator+(const Vector &summand) const {
    return {static_cast<int8_t>(x + summand.x),
            static_cast<int8_t>(y + summand.y)};
}

Vector & Vector::operator+=(const Vector &vector) {
    this->x = static_cast<int8_t>(this->x + vector.x);
    this->y = static_cast<int8_t>(this->y + vector.y);
    return *this;
}

Vector Vector::operator-(const Vector &summand) const {
    return {static_cast<int8_t>(x + summand.x),
            static_cast<int8_t>(y + summand.y)};
}

Vector & Vector::operator-=(const Vector &vector) {
    this->x = static_cast<int8_t>(this->x - vector.x);
    this->y = static_cast<int8_t>(this->y - vector.y);
    return *this;
}

bool Vector::operator==(const Vector &other) const {
    return x == other.x && y == other.y;
}

Vector Vector::mirrorHorizontal() const { return Vector(static_cast<int8_t>(-x), y); }

Vector Vector::mirrorVertical() const { return Vector(x, static_cast<int8_t>(-y)); }

Vector Vector::rotate90(const bool clockwise) const {
    return Vector(clockwise ? y : static_cast<int8_t>(-y),
                  clockwise ? static_cast<int8_t>(-x) : x);
}

std::vector<Vector> Vector::getAllPossibleTransforms() const {
    // Treat Vector as a shape and return all possible Vectors with the same
    // shape.
    if (x == y || y == 0 || x == 0) {
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

bool Vector::isWithinGrid() const {
    return this->x >= 0 && this->x < 8 && this->y >= 0 && this->y < 8;
}
