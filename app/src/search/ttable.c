/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "search/ttable.h"
#include "chess/board.h"
#include "chess/move.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include "xxHash/xxhash.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#if SWITCH_OPENMP
#include <omp.h>
#endif

struct TTable
{
	size_t num_blocks;
	size_t block_size;
	struct TTableNode (*(*blocks)[])[];
};

struct TTable *
ttable_new(void)
{
	struct TTable *ttable = xmalloc(sizeof(struct TTable));
	ttable->block_size = SWITCH_TTABLE_BLOCK_SIZE / sizeof(struct TTableNode) *
	                     sizeof(struct TTableNode);
	ttable->num_blocks = 1;
	ttable->blocks = xmalloc(sizeof(struct TTableNode *));
	(*ttable->blocks)[0] = xmalloc(ttable->block_size);
	return ttable;
}

void
ttable_free(struct TTable *ttable)
{
	assert(ttable);
	free(ttable);
}

struct TTableNode *
ttable_probe(struct TTable *ttable, uint64_t key)
{
	assert(ttable);
	/**
	 * ttable->num_blocks and ttable->block_size are assumed to be a power of 2.
	 */
	size_t i = key & (ttable->num_blocks - 1);
	size_t j = (key >> (ttable->num_blocks - 1)) & (ttable->block_size - 1);
	return &(*(*ttable->blocks)[i])[j];
}

struct TTableNode *
ttable_insertion(struct TTable *ttable, uint64_t key)
{
	assert(ttable);
	size_t i = key & (ttable->num_blocks - 1);
	size_t j = (key >> (ttable->num_blocks - 1)) & (ttable->block_size - 1);
	return &(*(*ttable->blocks)[i])[j];
}

void
ttable_clear(struct TTable *ttable)
{
	assert(ttable);
#if SWITCH_OPENMP
#pragma omp parallel for
#endif
	for (size_t i = 0; i < ttable->num_blocks; i++) {
		free((*ttable->blocks)[i]);
	}
	free(ttable->blocks);
	ttable->blocks = xmalloc(sizeof(struct TTableNode *));
	(*ttable->blocks)[0] = xmalloc(ttable->block_size);
}
