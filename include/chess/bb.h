#ifndef BB_H
#define BB_H

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

extern Bitboard BB_KNIGHT[64];
extern Bitboard BB_KING[64];

extern Bitboard BB_BISHOP_6[64];
extern Bitboard BB_ROOK_6[64];

extern const Bitboard MAGIC_BISHOP[64];
extern const Bitboard MAGIC_ROOK[64];

extern const int SHIFT_BISHOP[64];
extern const int SHIFT_ROOK[64];

extern int OFFSET_BISHOP[64];
extern int OFFSET_ROOK[64];

extern Bitboard ATTACK_BISHOP[5248];
extern Bitboard ATTACK_ROOK[102400];

extern Bitboard HASH_WHITE_PAWN[64];
extern Bitboard HASH_BLACK_PAWN[64];
extern Bitboard HASH_WHITE_KNIGHT[64];
extern Bitboard HASH_BLACK_KNIGHT[64];
extern Bitboard HASH_WHITE_BISHOP[64];
extern Bitboard HASH_BLACK_BISHOP[64];
extern Bitboard HASH_WHITE_ROOK[64];
extern Bitboard HASH_BLACK_ROOK[64];
extern Bitboard HASH_WHITE_QUEEN[64];
extern Bitboard HASH_BLACK_QUEEN[64];
extern Bitboard HASH_WHITE_KING[64];
extern Bitboard HASH_BLACK_KING[64];
extern Bitboard HASH_CASTLE[16];
extern Bitboard HASH_EP[8];
extern Bitboard HASH_COLOR;

void
bb_init();
void
bb_print(Bitboard value);
Bitboard
bb_random();

Bitboard
bb_bishop(int sq, Bitboard obstacles);
Bitboard
bb_rook(int sq, Bitboard obstacles);
Bitboard
bb_queen(int sq, Bitboard obstacles);

#endif
