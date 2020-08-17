#include "base64/base64.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core/eval.h"
#include "engine.h"
#include "eval.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// State for search agents. It holds a game-tree several plies deep.
struct SSearchStack
{
	struct SSearchStackPlie *plies;
	int desired_depth;
	int current_depth;
	struct Board board;
};

struct SSearchStackPlie
{
	struct Move generator;
	struct Move *moves;
	int children_count;
	int child_i;
	int best_child_i_so_far;
	float best_eval_so_far;
};

struct SSearchStack
ssearch_stack_new(const struct Engine *engine)
{
	assert(engine->max_depth > 0);
	struct SSearchStack stack;
	stack.plies = exit_if_null(malloc(engine->max_depth * sizeof(struct SSearchStackPlie)));
	stack.desired_depth = engine->max_depth - 1;
	stack.current_depth = 0;
	stack.board = engine->board;
	for (int i = 0; i <= stack.desired_depth; i++) {
		stack.plies[i].moves = exit_if_null(malloc(220 * sizeof(struct Move)));
	}
	stack.plies[0].child_i = 0;
	stack.plies[0].best_child_i_so_far = 0;
	stack.plies[0].children_count = gen_legal_moves(stack.plies[0].moves, &stack.board);
	stack.plies[0].best_eval_so_far = -FLT_MAX;
	return stack;
}

void
ssearch_stack_delete(struct SSearchStack *stack)
{
	for (int i = 0; i <= stack->desired_depth; i++) {
		free(stack->plies[i].moves);
	}
	free(stack->plies);
}

struct SSearchStackPlie *
ssearch_stack_last(struct SSearchStack *stack)
{
	return stack->plies + stack->current_depth;
}

float
normalize_score(float score, enum Color side_to_move)
{
	const float multipliers[COLORS_COUNT] = { 1.0, -1.0 };
	return score * multipliers[side_to_move];
}

void
ssearch_stack_pop(struct SSearchStack *stack)
{
	struct SSearchStackPlie *last_plie = ssearch_stack_last(stack);
	float eval = last_plie->best_eval_so_far;
	position_undo_move_and_flip(&stack->board, &last_plie->generator);
	stack->current_depth--;
	last_plie--;
		if (stack->current_depth == 0) {
		}
	if (-eval > last_plie->best_eval_so_far) {
		last_plie->best_eval_so_far = -eval;
		last_plie->best_child_i_so_far = last_plie->child_i;
	}
	last_plie->child_i++;
}

void
ssearch_stack_push(struct SSearchStack *stack)
{	
	struct SSearchStackPlie *last_plie = ssearch_stack_last(stack);
	assert(last_plie->child_i < last_plie->children_count);
	struct Move generator = last_plie->moves[last_plie->child_i];
	stack->current_depth++;
	last_plie++;
	last_plie->child_i = 0;
	last_plie->best_child_i_so_far = -1;
	last_plie->best_eval_so_far = -FLT_MAX;
	last_plie->generator = generator;
	position_do_move_and_flip(&stack->board, &last_plie->generator);
	last_plie->children_count = gen_legal_moves(last_plie->moves, &stack->board);
}

void
ssearch_stack_iter(struct SSearchStack *stack)
{
	struct SSearchStackPlie *last_plie = ssearch_stack_last(stack);
	enum Color evaluator = stack->board.side_to_move;
	position_do_move_and_flip(&stack->board, &last_plie->moves[last_plie->child_i]);
	float eval = normalize_score(position_eval(&stack->board), evaluator);
	position_undo_move_and_flip(&stack->board, &last_plie->moves[last_plie->child_i]);
	if (eval > last_plie->best_eval_so_far) {
		last_plie->best_eval_so_far = eval;
		last_plie->best_child_i_so_far = last_plie->child_i;
	}
	last_plie->child_i++;
}

struct SearchResults
{
	struct Move best_move;
	struct Move ponder_move;
	float centipawns;
};

void
engine_start_search(struct Engine *engine)
{
	engine->max_depth = 4;
	fprintf(engine->output, "info depth score cp %f\n", position_eval(&engine->board));
	struct SSearchStack stack = ssearch_stack_new(engine);
	while (true) {
		struct SSearchStackPlie *last_plie = ssearch_stack_last(&stack);
		// We use depth-first search (DFS) to explore the game tree.
		if (last_plie->child_i == last_plie->children_count) {
			if (stack.current_depth == 0) {
				break;
			} else {
				ssearch_stack_pop(&stack);
			}
		} else if (stack.current_depth == stack.desired_depth) {
			ssearch_stack_iter(&stack);
		} else {
			ssearch_stack_push(&stack);
		}
	}
	struct SearchResults result = {
		.best_move = stack.plies[0].moves[stack.plies[0].best_child_i_so_far],
		// FIXME: `.ponder_move` is the best-scoring move in the subtree of the
		// best-scoring move of the first plie.
		.ponder_move = stack.plies[1].best_child_i_so_far,
		.centipawns = stack.plies[0].best_eval_so_far,
	};
	char buf[MOVE_STRING_MAX_LENGTH] = { '\0' };
	move_to_string(result.best_move, buf);
	printf("bestmove %s\n", buf);
}

struct Move
engine_search_with_depth(struct Engine *engine)
{
	struct Move moves[MAX_MOVES] = { 0 };
	size_t count = gen_legal_moves(moves, &engine->board);
}

void
engine_stop_search(struct Engine *engine)
{
	assert(engine);
	ENGINE_LOGF(engine, "[INFO] Interrupting search.\n");
}

void
agent_delete(struct Agent *agent)
{
	free(agent);
}
