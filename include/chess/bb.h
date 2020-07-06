#ifndef ZORRO_CHESS_BB_H
#define ZORRO_CHESS_BB_H

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

extern Bitboard BB_KNIGHT_ATTACKS[64];
extern Bitboard BB_KING_ATTACKS[64];
extern const Bitboard MAGIC_BISHOP[64];
extern const Bitboard MAGIC_ROOK[64];

extern Bitboard ATTACK_BISHOP[5248];
extern Bitboard ATTACK_ROOK[10240];

void
bb_init(void);
void
bb_print(Bitboard value);
Bitboard
bb_random();

Bitboard
bb_bishop(Square sq, Bitboard obstacles);
Bitboard
bb_rook(Square sq, Bitboard obstacles);

#endif
