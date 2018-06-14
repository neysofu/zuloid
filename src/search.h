#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "move.h"

struct SearchPriorityQueue;

struct SearchPriorityQueue *
search_priority_queue_new(struct SearchPriorityQueue *spq);

void
search_priority_queue_drop(struct SearchPriorityQueue *spq);

uint64_t
search_priority_queue_pop(struct SearchPriorityQueue *spq);

void
search_priority_queue_push(struct SearchPriorityQueue *spq, float priority, uint64_t value);

struct SearchParams {
	bool ponder;
	uint16_t moves_to_go;
	uint16_t depth;
	size_t num_nodes;
	uint16_t mate_length;
	bool infinite;
};

struct EvalMove {
	struct Move move;
	float score;
};

struct Leaf {
	struct Board *board;
	float score;
};

struct Node {
	struct Move move;
};

struct Scheduler {
	struct Node *tasks;
};
