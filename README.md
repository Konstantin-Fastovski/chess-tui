# chess-tui

Ein im Terminal spielbares Schachspiel gegen einen Gegner im Hot-Seat oder gegen einen simplen Computer-Gegner.

## Zugformat

Normale Züge: a2a3, f1c4, etc.
Rochaden: O-O, O-O-O
Spiel speichern/laden: s/l (Auch wärend dem Spiel möglich) -> Gespeichert in chess.data im selben directory

## Fehlende Features

Promotion & En passant

## Programmentwurf & Kernideen
- Figuren als Klassen (piece.hpp/cpp)
- Board, das Figuren enthält (board.hpp/cpp)
- Visitorpattern zum Berechnen der erreichbaren Zellen einer Figur, um zyklische Abhängigkeit zwischen Board und Figur zu vermeiden (Figur als Datenorientierte Klasse) (piece-visitor.hpp/cpp)
- Abstrakter Spieler und Implementationen für lokale Spieler und einfache Bots (player.hpp/cpp)
- Einfache Vektorimplementation für das Spiel (vector.hpp/cpp)

In der main loop (main.cpp) werden bis ein Spieler gewonnen hat Spielzüge abgefragt, geparst, ausgeführt und geprüft.


## Binary-Format
| Offset | Size | Description     |
|--------|------|-----------------|
| 0      | 1    | Piece-Count     |
| 1      | n*4  | n * Piece-Data  |

n = Piece-Count

### Piece-Data
| Offset | Size | Content   | Description                                      |
|--------|------|-----------|--------------------------------------------------|
| 0      | 1    | x         | Unsigned 1-Byte Integer with values from 0 to 7  |
| 1      | 1    | y         | Unsigned 1-Byte Integer with values from 0 to 7  |
| 2      | 1    | white     | Bool - 1 if piece is white, 0 otherwise          |
| 3      | 1    | has_moved | Bool - 1 if piece has already moved, 0 otherwise |
