#pragma once

#include <stdlib.h>
#include "move.h"

struct MoveStack {
	size_t size;
	size_t capacity;
	Move (*moves)[];
};

struct MoveStack *
move_stack_new(void);

void
move_stack_drop(struct MoveStack *move_stack);

void
move_stack_push(struct MoveStack *move_stack, Move move);
