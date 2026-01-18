//
// Created by felix on 1/9/26.
//

#include "chess-tui/piece.hpp"

std::string Pawn::getUnicode() {
    return this->white ? "\u265F" : "\u2659";
}

void Pawn::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char Pawn::getSymbol() {
    return 'P';
}

std::string Rook::getUnicode() {
    return this->white ? "\u265C" : "\u2656";
}

void Rook::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char Rook::getSymbol() {
    return 'R';
}

std::string Knight::getUnicode() {
    return this->white ? "\u265E" : "\u2658";
}

void Knight::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char Knight::getSymbol() {
    return 'N';
}

std::string King::getUnicode() {
    return this->white ? "\u265A" : "\u2654";
}

void King::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char King::getSymbol() {
    return 'K';
}

std::string Bishop::getUnicode() {
    return this->white ? "\u265D" : "\u2657";
}

void Bishop::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char Bishop::getSymbol() {
    return 'B';
}

std::string Queen::getUnicode() {
    return this->white ? "\u265B" : "\u2655";
}

void Queen::visit(PieceVisitor &pieceVisitor) {
    pieceVisitor.visit(*this);
}

char Queen::getSymbol() {
    return 'Q';
}
