/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "search/scontroller.h"
#include "search/ttable_node.h"
#include "chess/move.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>

struct SController
{
	struct TTable *ttable;
};

struct SController *
scontroller_new(struct TTable *ttable)
{
	assert(ttable);
	struct SController *scontroller = xmalloc(sizeof(struct SController));
	scontroller->ttable = ttable;
	return scontroller;
}

void
scontroller_free(struct SController *scontroller)
{
	assert(scontroller);
	free(scontroller);
}

void
scontroller_start(struct SController *scontroller, struct Board *board)
{
	assert(scontroller);
	assert(board);
#if SWITCH_OPENMP
#pragma omp task
#endif
	struct TTableNode *root = NULL;//ttable_get(scontroller->ttable, board);
	size_t i = 8;
	while (i-- > 0) {
		struct TTableNode *node = ttable_node_rnd_child(root, 0);
	}
}

Move
scontroller_stop(struct SController *scontroller)
{
	assert(scontroller);
	return MOVE_NONE;
}
