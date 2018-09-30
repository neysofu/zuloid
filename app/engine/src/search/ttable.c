#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "chess/board.h"
#include "chess/move.h"
#include "globals.h"
#include "switches.h"
#include "search/ttable.h"
#include "xxHash/xxhash.h"


struct TTableElement {
	struct Board board;
	struct TTableElement *moves;
};

struct TTable {
	size_t element_size;
	uint_fast8_t fragment_right_shift;
	uint_fast8_t element_right_shift;
	size_t num_fragments;
	struct TTableElement (*(*fragments)[])[];
};


struct TTable *
ttable_new(void) {
	struct TTable *ttable = malloc(sizeof(struct TTable));
	if (!ttable) {
		return NULL;
	}
	ttable->fragment_right_shift = log2(SWITCH_TTABLE_FRAGMENT_SIZE);
	ttable->element_right_shift = log2(sizeof(struct TTableElement));
	ttable->fragments = malloc(sizeof(void *));
	if (!ttable->fragments) {
		goto drop_after_fragments;
	}
	(*ttable->fragments)[0] = malloc(SWITCH_TTABLE_FRAGMENT_SIZE);
	if (!(*ttable->fragments)[0]) {
		goto drop_after_first_fragment;
	}
	return ttable;

drop_after_first_fragment:
	free(ttable->fragments);
drop_after_fragments:
	free(ttable);
drop_after_ttable:
	return NULL;
}

void
ttable_drop(struct TTable *ttable) {
	assert(ttable);
	ttable_clear_fragments(ttable);
	free(ttable->fragments);
	free(ttable);
}

void *
ttable_ith_fragment(struct TTable *ttable, size_t i) {
	return (*ttable->fragments)[ttable->num_fragments];
}

void
ttable_clear_fragments(struct TTable *ttable) {
	assert(ttable);
	while (ttable->num_fragments-- > 0) {
		free(ttable_ith_fragment(ttable, ttable->num_fragments));
	}
}

void *
ttable_element(struct TTable *ttable, struct Board *board) {
	assert(ttable);
	assert(board);
#if SWITCH_HASH == SWITCH_HASH_XXH64
	uint64_t hash = XXH64(board, sizeof(struct Board), 0);
#else
	assert(false);
#endif
	size_t i_fragment = hash >> ttable->fragment_right_shift;
	size_t i_element = hash << ttable->element_right_shift;
	void *fragment = (&ttable->fragments)[i_fragment];
	return fragment + i_element * ttable->element_size;
}

void
ttable_insert(struct TTable *ttable, struct Board *board) {
	// TODO
}

Move
ttable_best_move(struct TTable *ttable) {
	return MOVE_NONE; // TODO
}
