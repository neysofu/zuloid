/* SPDX-License-Identifier: GPL-3.0-only */

#include "chess/bb.h"
#include "chess/color.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core/sstack.h"
#include "meta.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef unsigned long long perft_counter;

struct DfsStack
{
	struct PlieIter *plies;
	int desired_depth;
	int current_depth;
	struct Board board;
};

struct DfsStack
dfsstack_new(size_t depth)
{
	struct DfsStack stack;
	stack.plies = exit_if_null(malloc((depth + 1) * sizeof(struct PlieIter)));
	stack.desired_depth = depth;
	stack.current_depth = 1;
	for (size_t i = 0; i < depth + 1; i++) {
		plieiter_init(&stack.plies[i]);
	}
	return stack;
}

void
dfsstack_delete(struct DfsStack *stack)
{
	for (int i = 0; i < stack->desired_depth; i++) {
		plieiter_delete(&stack->plies[i]);
	}
	free(stack->plies);
}

struct PlieIter *
dfsstack_last(const struct DfsStack *stack)
{
	return stack->plies + stack->current_depth;
}

void
dfsstack_pop(struct DfsStack *stack)
{
	struct PlieIter *last_plie = dfsstack_last(stack);
	assert(stack->current_depth > 0);
	assert(!plieiter_has_next(last_plie));
	position_undo_move_and_flip(&stack->board, &last_plie->generator);
	stack->current_depth--;
}

void
dfsstack_push(struct DfsStack *stack)
{
	struct PlieIter *last_plie = dfsstack_last(stack);
	assert(plieiter_has_next(last_plie));
	struct Move generator = last_plie->moves[last_plie->child_i++];
	stack->current_depth++;
	last_plie++;
	last_plie->child_i = 0;
	last_plie->generator = generator;
	position_do_move_and_flip(&stack->board, &last_plie->generator);
	last_plie->children_count = gen_legal_moves(last_plie->moves, &stack->board);
}

unsigned long long
dfsstack_count(struct DfsStack *stack)
{
	unsigned long long result = 0;
	while (true) {
		struct PlieIter *last_plie = dfsstack_last(stack);
		if (!plieiter_has_next(last_plie)) {
			if (stack->current_depth == 1) {
				return result;
			} else {
				dfsstack_pop(stack);
			}
		} else if (stack->current_depth == stack->desired_depth) {
			last_plie->child_i++;
			result++;
		} else {
			dfsstack_push(stack);
		}
	}
}

size_t
position_improved_perft(struct Board *pos, unsigned depth)
{
	if (depth == 0) {
		return 1;
	} else {
		struct DfsStack stack = dfsstack_new(depth);
		stack.board = *pos;
		stack.plies[1].child_i = 0;
		stack.plies[1].children_count = gen_legal_moves(stack.plies[1].moves, &stack.board);
		return dfsstack_count(&stack);
	}
}

size_t
position_perft(FILE *stream, const struct Board *pos, size_t depth)
{
	if (depth == 0) {
		fputs("1\n", stream);
		return 1;
	} else if (depth > MAX_DEPTH) {
		fprintf(stream, "<depth limit exceeded>\n");
		return 0;
	}
	struct Move *moves = exit_if_null(malloc(sizeof(struct Move) * MAX_MOVES));
	size_t root_moves_count = gen_legal_moves(moves, pos);
	perft_counter result = 0;
	for (size_t i = 0; i < root_moves_count; i++) {
		struct Board board = *pos;
		position_do_move_and_flip(&board, moves + i);
		perft_counter children_count = position_improved_perft(&board, depth - 1);
		result += children_count;
		char mv_as_str[MOVE_STRING_MAX_LENGTH] = { '\0' };
		move_to_string(moves[i], mv_as_str);
		fprintf(stream, "%s: %llu\n", mv_as_str, children_count);
	}
	fprintf(stream, "\nNodes searched: %llu\n\n", result);
	free(moves);
	return result;
}
