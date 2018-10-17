#include "search/ttable.h"
#include "chess/board.h"
#include "chess/move.h"
#include "globals.h"
#include "search/ttable_node.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct TTable
{
	size_t element_size;
	size_t fragment_size;
	uint32_t fragment_right_shift;
	uint32_t element_right_shift;
	size_t num_fragments;
	struct TTableNode (*(*fragments)[])[];
	struct TTableNode *root;
};

struct TTable *
ttable_new(void)
{
	struct TTable *ttable = xmalloc(sizeof(struct TTable));
	ttable->fragment_right_shift = log2(SWITCH_TTABLE_FRAGMENT_SIZE);
	ttable->element_right_shift = log2(sizeof(struct TTableNode));
	ttable->fragments = xmalloc(sizeof(void *));
	(*ttable->fragments)[0] = xmalloc(SWITCH_TTABLE_FRAGMENT_SIZE);
	return ttable;
}

void
ttable_free(struct TTable *ttable)
{
	if (!ttable) {
		debug_printf("Freeing a NULL pointer.\n");
		return;
	}
	free(ttable->fragments);
	free(ttable);
}

void
ttable_clear(struct TTable *ttable)
{
	assert(ttable);
	for (size_t i = ttable->num_fragments; i > 0; i--) {
		memset(*(*ttable->fragments)[i], 0, ttable->fragment_size);
	}
}

struct TTableNode *
ttable_get(struct TTable *ttable, struct Board *board)
{
	assert(ttable);
	assert(board);
	uint64_t hash = XXH64(board, sizeof(struct Board), 0);
	struct TTableNode *node = ttable->root;
	while (node->hash != hash) {
		if (node->hash < hash) {
			node = node->lnode;
		} else {
			node = node->rnode;
		}
		assert(node);
	}
	return node;
}

struct TTableNode *
ttable_select(struct TTable *ttable, struct TTableNode *parent)
{
	return parent; // TODO
}

void
ttable_backpropagate(struct TTable *ttable,
                     struct TTableNode *parent,
                     struct TTableNode *child)
{
	// TODO
}

void
ttable_expand(struct TTable *ttable, struct TTableNode *parent, Move move)
{
	// TODO
}

Move
ttable_search(struct TTable *ttable, struct Board *board)
{
	assert(ttable);
	assert(board);
	struct TTableNode *node = ttable_rnd(ttable);
	ttable_backpropagate(ttable, node, 0.5);
	return MOVE_NONE;
}
