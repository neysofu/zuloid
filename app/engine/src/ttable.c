#include <stdlib.h>
#include "board.h"
#include "move.h"
#include "switches.h"
#include "ttable.h"
#include "xxHash/xxhash.h"

struct TTableItem {
	struct Board board;
	uint32_t count;
	Move moves[BOARD_AVERAGE_NUM_CANDIDATE_MOVES];
};

struct TTable {
	size_t num_buckets;
	uint_fast8_t bucket_left_shift;
	uint_fast8_t item_left_shift;
	struct TTableItem (*(*buckets)[])[TTABLE_BUCKET_SIZE];
};

struct TTable *
ttable_new(void) {
	struct TTable *ttable = malloc(sizeof(struct TTable));
	ttable->num_buckets = TTABLE_ITEM_INITIAL_NUM_BUCKETS;
	ttable->buckets = malloc(TTABLE_BUCKET_SIZE *
			                 TTABLE_ITEM_INITIAL_NUM_BUCKETS);
	return ttable;
}

void
ttable_drop(struct TTable *ttable) {
	ttable_clear(ttable);
	free(ttable);
}

void
ttable_clear(struct TTable *ttable) {
	size_t new_num_buckets = ttable->num_buckets << 1;
	while (ttable->num_buckets-- < new_num_buckets) {
		free((*ttable->buckets)[ttable->num_buckets]);
	}
}

struct TTableItem *
ttable_ith(struct TTable *ttable, size_t i) {
	return (&(&ttable->buckets)[i << ttable->bucket_left_shift])
		   [i << ttable->item_left_shift];
}


void
ttable_insert(struct TTable *ttable, struct Board *board) {
#if Z64C_SWITCH_HASH == Z64C_SWITCH_HASH_XXH64
	uint64_t i = XXH64(board, sizeof(struct Board), 0);
	struct TTableItem *item = ttable_ith(ttable, i);
	item->board = *board;
	item->count++;
#else
#error "Unsupported hash function."
#endif
}

void
ttable_resize(struct TTable *ttable) {
	size_t new_num_buckets = ttable->num_buckets << 1;
	ttable->buckets = realloc(ttable->buckets, new_num_buckets * TTABLE_BUCKET_SIZE);
	do {
		// TODO
		//(*ttable->buckets)[ttable->num_buckets] = ttable_bucket_new();
	} while (ttable->num_buckets++ < new_num_buckets);
}

Move
ttable_best_move(struct TTable *ttable) {
	return MOVE_NONE; // FIXME
}

void
ttable_prune_useless_lines(struct TTable *ttable, Move move) {

}
