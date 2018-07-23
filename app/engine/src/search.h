#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "move.h"

struct SearchPriorityQueue;

struct SearchPriorityQueue *
search_priority_queue_new(struct SearchPriorityQueue *spq);

void
search_priority_queue_drop(struct SearchPriorityQueue *spq);

struct Move
search_priority_queue_pop_move(struct SearchPriorityQueue *spq);

void
search_priority_queue_grow(struct SearchPriorityQueue *spq);

struct SearchParams {
	uint16_t max_depth;
	uint64_t max_num_nodes;
	struct Clock *clock;
};

struct SearchParams *
search_params_new(struct SearchParams *search_params);

void
search_params_drop(struct SearchParams *search_params);
