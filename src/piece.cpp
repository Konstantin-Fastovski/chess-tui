//
// Created by felix on 1/9/26.
//

#include "chess-tui/piece.hpp"

std::string Pawn::getUnicode() {
    return this->white ? "\u2659" : "\u265F";
}

void Pawn::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

std::string Rook::getUnicode() {
    return this->white ? "\u2656" : "\u265C";
}

void Rook::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}
std::string Knight::getUnicode() {
    return this->white ? "\u2658" : "\u265E";
}

void Knight::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

std::string King::getUnicode() {
    return this->white ? "\u2654" : "\u265A";
}

void King::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

std::string Bishop::getUnicode() {
    return this->white ? "\u2657" : "\u265D";
}

void Bishop::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

std::string Queen::getUnicode() {
    return this->white ? "\u2655" : "\u265B";
}

void Queen::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}
