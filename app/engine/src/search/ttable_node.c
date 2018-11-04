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
