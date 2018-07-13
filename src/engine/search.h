#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "move.h"

struct SearchPriorityQueue;

struct SearchNode;

struct SearchNode *
search_node_new(struct SearchNode *s_node);

void
search_node_drop(struct SearchNode *s_node);

struct SeachNode *
search_node_expand(struct SearchNode *s_node);


struct SearchPriorityQueue *
search_priority_queue_new(struct SearchPriorityQueue *spq);

void
search_priority_queue_drop(struct SearchPriorityQueue *spq);

struct Move
search_priority_queue_pop_move(struct SearchPriorityQueue *spq);

void
search_priority_queue_push(struct SearchPriorityQueue *spq, struct SearchNode *s_node, float priority);

struct SearchParams {
	uint16_t max_depth;
	uint64_t max_num_nodes;
};
