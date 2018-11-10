/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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

#if SWITCH_OPENMP
#include <omp.h>
#endif

struct TTable
{
	size_t fragment_size;
	size_t num_fragments;
	struct TTableNode (*fragments)[];
};

struct TTable *
ttable_new(void)
{
	struct TTable *ttable = xmalloc(sizeof(struct TTable));
//	ttable->fragment_right_shift = log2(SWITCH_TTABLE_FRAGMENT_SIZE);
//	ttable->element_right_shift = log2(sizeof(struct TTableNode));
//	ttable->fragments = xmalloc(sizeof(void *));
//	(*ttable->fragments)[0] = xmalloc(SWITCH_TTABLE_FRAGMENT_SIZE);
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
	return NULL;
}

void
ttable_clear(struct TTable *ttable)
{
	assert(ttable);
	/**
	 * See
	 * https://stackoverflow.com/questions/11576670/in-an-openmp-parallel-code-would-there-be-any-benefit-for-memset-to-be-run-in-p
	 * for performance considerations.
	 */
#if SWITCH_OPENMP
#pragma omp parallel for
#endif
	for (size_t i = ttable->num_fragments; i > 0; i--) {
		//memset(*(*ttable->fragments)[i], 0, ttable->fragment_size);
	}
}
