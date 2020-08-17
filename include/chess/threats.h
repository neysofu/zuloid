#ifndef ZULOID_CHESS_THREATS_H
#define ZULOID_CHESS_THREATS_H

#include "chess/coordinates.h"

void
init_threats(void);

Bitboard
threats_by_king(Square sq);

Bitboard
threats_by_knight(Square sq);

Bitboard
threats_by_rook(Square sq, Bitboard obstacles);
Bitboard
threats_by_rook_no_init(Square sq, Bitboard obstacles);

Bitboard
threats_by_bishop(Square sq, Bitboard obstacles);
Bitboard
threats_by_bishop_no_init(Square sq, Bitboard obstacles);

#endif
