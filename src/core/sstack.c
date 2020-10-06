/* SPDX-License-Identifier: GPL-3.0-only */

#include "core/sstack.h"
#include "utils.h"
#include <stdbool.h>

bool
plieiter_has_next(const struct PlieIter *plie)
{
	return plie->child_i < plie->children_count;
}

void
plieiter_init(struct PlieIter *plie)
{
	plie->moves = exit_if_null(malloc(220 * sizeof(struct Move)));
	plie->child_i = 0;
}

void
plieiter_delete(struct PlieIter *plie)
{
	free(plie->moves);
	plie->moves = NULL;
}
