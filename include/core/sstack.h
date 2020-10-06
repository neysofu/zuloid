/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_CORE_SSTACK_H
#define ZULOID_CORE_SSTACK_H

#include "chess/move.h"
#include <stdbool.h>

struct PlieIter
{
	struct Move generator;
	struct Move *moves;
	int children_count;
	int child_i;
};

bool
plieiter_has_next(const struct PlieIter *plie);

void
plieiter_init(struct PlieIter *plie);

void
plieiter_delete(struct PlieIter *plie);

#endif
