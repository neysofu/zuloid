#ifndef ZULOID_CHESS_BB_H
#define ZULOID_CHESS_BB_H

#include "chess/coordinates.h"

#define BIT(sq) (1L << (sq))
#define RF(rank, file) ((rank)*8 + (file))

#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))
#define BITS(x) (__builtin_popcountll(x))

#define POP_LSB(b, x)                                                                      \
	b = LSB(x);                                                                            \
	x &= ~BIT(b);
#define POP_MSB(b, x)                                                                      \
	b = MSB(x);                                                                            \
	x &= ~BIT(b);

extern Bitboard BB_ATTACKS_BY_KNIGHT[64];
extern Bitboard BB_ATTACKS_BY_KING[64];

Bitboard
bb_next_subset(Bitboard mask, Bitboard previous_subset);

void
bb_init(void);
void
bb_pprint(Bitboard value);

Bitboard
bb_bishop(Square sq, Bitboard obstacles);
Bitboard
bb_rook(Square sq, Bitboard obstacles);

#endif
