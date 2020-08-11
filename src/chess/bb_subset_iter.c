#include "chess/bb_subset_iter.h"
#include <stdlib.h>

struct BitboardSubsetIter
bb_subset_iter_from_mask(Bitboard mask) {
	return (struct BitboardSubsetIter) {
		.original = mask,
		.subset = 0,
	};
}

Bitboard *
bb_subset_iter(struct BitboardSubsetIter *iter)
{
	// https://www.chessprogramming.org/Traversing_Subsets_of_a_Set
	iter->subset = (iter->subset - iter->original) & iter->original;
	if (iter->subset == 0) {
		return NULL;
	} else {
		return &(iter->subset);
	}
}
