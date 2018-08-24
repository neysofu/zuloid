#include <stdlib.h>
#include "move_stack.h"
#include "move.h"

struct MoveStack {
	std::stack<struct Move> stack;
};

struct MoveStack *
move_stack_new(void) {
	return (struct MoveStack) {
		.stack = std::stack<struct Move>,
	};
}

void
move_stack_drop(struct MoveStack *move_stack) {
	delete move_stack->stack;
	free(move_stack);
}

size_t
move_stack_size(struct MoveStack *move_stack) {
	return move_stack->stack.size();
}

void
move_stack_push(struct MoveStack *move_stack, struct Move move) {
	move_stack->stack.push_back(move);
}
