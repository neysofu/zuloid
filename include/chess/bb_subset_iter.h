#ifndef ZORRO_CHESS_BB_SUBSET_ITER_H
#define ZORRO_CHESS_BB_SUBSET_ITER_H

#include "chess/coordinates.h"

struct BitboardSubsetIter
{
	Bitboard original;
	Bitboard subset;
};

Bitboard *
bb_subset_iter(struct BitboardSubsetIter *iter);

#endif
