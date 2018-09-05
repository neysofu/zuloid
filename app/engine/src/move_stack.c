#include <stdlib.h>
#include "move_stack.h"
#include "move.h"

struct MoveStack *
move_stack_new(void) {
	struct MoveStack *move_stack = malloc(sizeof(struct MoveStack));
	move_stack->size = 1;
	move_stack->capacity = 1;
	move_stack->moves = malloc(sizeof(Move));
	return move_stack;
}

void
move_stack_drop(struct MoveStack *move_stack) {
	free(move_stack->moves);
	free(move_stack);
}

void
move_stack_push(struct MoveStack *move_stack, Move move) {
	if (move_stack->size == move_stack->capacity) {
		move_stack->capacity <<= 1;
		move_stack->moves = realloc(move_stack->moves,
				                    sizeof(Move) * move_stack->capacity);
	}
	(*move_stack->moves)[move_stack->size++] = move;
}
