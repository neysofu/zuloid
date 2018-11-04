/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "search/ttable_node.h"
#include <stdint.h>
#include <stdlib.h>

void
ttable_node_backpropagate(struct TTableNode *parent,
                          struct TTableNode *child,
                          bool *lock);

struct TTableNode *
ttable_node_rnd_child(struct TTableNode *ttnode, uint32_t seed)
{
//	return ttnode->children[(int)(ttnode->num_children * seed)];
	return NULL;
}
