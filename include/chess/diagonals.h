#ifndef ZORRO_CHESS_DIAGONALS_H
#define ZORRO_CHESS_DIAGONALS_H

#include "chess/coordinates.h"

extern const Bitboard DIAGONALS_A1H8[15];
extern const Bitboard DIAGONALS_A8H1[15];

Bitboard
square_a1h8_diagonal(Square sq);

Bitboard
square_a8h1_diagonal(Square sq);

#endif
