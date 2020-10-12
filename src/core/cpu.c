/* SPDX-License-Identifier: GPL-3.0-only */

#include "base64/base64.h"
#include "cJSON/cJSON.h"
#include "cache/cache.h"
#include "chess/fen.h"
#include "chess/mnemonics.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "core/eval.h"
#include "core/sstack.h"
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
struct SStack
{
	struct SStackPlieIter *plies;
	struct Cache *cache;
	int desired_depth;
	int plie_i;
	struct Board board;
	float alpha;
	float beta;
};

struct SStackPlieIter
{
	int best_child_i_so_far;
	float best_eval_so_far;
	float multiplier;
	struct PlieIter iter;
};

void
ssplieiter_reset(struct SStackPlieIter *plie)
{
	plie->best_child_i_so_far = -1;
	plie->best_eval_so_far = -1000000.0;
	plie->iter.child_i = 0;
}

void
ssplieiter_init(struct SStackPlieIter *plie)
{
	plieiter_init(&plie->iter);
	ssplieiter_reset(plie);
}

void
ssplieiter_supply_eval(struct SStackPlieIter *plie, float eval)
{
	if (eval > plie->best_eval_so_far) {
		plie->best_eval_so_far = eval;
		plie->best_child_i_so_far = plie->iter.child_i;
	}
	plie->iter.child_i++;
}

int
depth_from_times(float wtime, float btime)
{
	UNUSED(wtime);
	UNUSED(btime);
	return 4; // TODO, this is a good default for now.
}

struct SStack
sstack_new(const struct Engine *engine)
{
	struct SStack stack;
	int max_depth = 
	  depth_from_times(engine->time_controls[COLOR_WHITE]->time_limit_in_seconds,
	                   engine->time_controls[COLOR_BLACK]->time_limit_in_seconds);
	stack.plies =
	  exit_if_null(malloc((max_depth + 1) * sizeof(struct SStackPlieIter)));
	stack.cache = engine->cache;
	stack.desired_depth = max_depth;
	stack.plie_i = 0;
	stack.board = engine->board;
	for (size_t i = 0; i <= (size_t)(max_depth); i++) {
		ssplieiter_init(&stack.plies[i]);
		stack.plies[i].multiplier =
		  ((engine->board.side_to_move == COLOR_WHITE) ^ (i % 2 == 1)) ? 1.0 : -1.0;
	}
	stack.plies[0].iter.children_count =
	  gen_legal_moves(stack.plies[0].iter.moves, &stack.board);
	return stack;
}

void
sstack_delete(struct SStack *stack)
{
	for (int i = 0; i <= stack->desired_depth; i++) {
		plieiter_delete(&stack->plies[i].iter);
	}
	free(stack->plies);
	stack->plies = NULL;
}

struct SStackPlieIter *
sstack_last(struct SStack *stack)
{
	return stack->plies + stack->plie_i;
}

void
sstack_print_lines(struct SStack *stack)
{
	printf("  info line");
	for (int i = 1; i < stack->desired_depth; i++) {
		char buf[6] = { '\0' };
		move_to_string(stack->plies[i].iter.moves[stack->plies[i].best_child_i_so_far],
		               buf);
		printf(" %s", buf);
	}
	puts("");
}

int
score_to_centipawns(float score)
{
	return (int)(score * 100);
}

void
sstack_log(const struct SStack *stack)
{
	for (int i = 0; i <= stack->plie_i; i++) {
		char buf[6] = { '\0' };
		move_to_string(stack->plies[i].iter.generator, buf);
		printf("%d %s %d\n", i, buf, stack->plies[i].iter.children_count);
	}
	position_pprint(&stack->board, stdout);
}

void
sstack_pop(struct SStack *stack)
{
	struct SStackPlieIter *last_plie = sstack_last(stack);
	float eval = last_plie->best_eval_so_far;
	position_undo_move_and_flip(&stack->board, &last_plie->iter.generator);
	stack->plie_i--;
	last_plie--;
	ssplieiter_supply_eval(last_plie, -eval);
	if (stack->plie_i == 0) {
		char buf[6] = { '\0' };
		move_to_string((last_plie + 1)->iter.generator, buf);
		printf("info depth 0 nodes %u currmove %s score cp %d\n",
		       0,
		       buf,
		       score_to_centipawns(eval));
	}
}

void
sstack_push(struct SStack *stack)
{
	struct SStackPlieIter *last_plie = sstack_last(stack);
	assert(plieiter_has_next(&last_plie->iter));
	struct Move generator = last_plie->iter.moves[last_plie->iter.child_i];
	stack->plie_i++;
	last_plie++;
	ssplieiter_reset(last_plie);
	last_plie->iter.generator = generator;
	position_do_move_and_flip(&stack->board, &last_plie->iter.generator);
	last_plie->iter.children_count = gen_legal_moves(last_plie->iter.moves, &stack->board);
	switch (last_plie->iter.children_count) {
		case 0: // stalemate
			last_plie->best_eval_so_far = 0.0;
			sstack_pop(stack);
			break;
		case -1: // checkmate
			last_plie->best_eval_so_far = last_plie->multiplier * 100000;
			sstack_pop(stack);
			break;
		default:
			break;
	}
}

void
ssplieiter_eval_leaf(struct SStackPlieIter *leaf, struct Board *board)
{
	position_do_move_and_flip(board, &leaf->iter.moves[leaf->iter.child_i]);
	float eval = position_eval(board) * leaf->multiplier;
	position_undo_move_and_flip(board, &leaf->iter.moves[leaf->iter.child_i]);
	ssplieiter_supply_eval(leaf, eval);
}

struct SearchResults
{
	struct Move best_move;
	struct Move ponder_move;
	float centipawns;
};

void
search_results_init(struct SearchResults *results, const struct SStack *stack)
{
	*results = (struct SearchResults){
		.best_move = stack->plies[0].iter.moves[stack->plies[0].best_child_i_so_far],
		// FIXME: `.ponder_move` is the best-scoring move in the subtree of the
		// best-scoring move of the first plie.
		.ponder_move = stack->plies[1].best_child_i_so_far,
		.centipawns = stack->plies[0].best_eval_so_far,
	};
}

void
finish_search(const struct Engine *engine, const struct SStack *stack)
{
	struct SearchResults result;
	search_results_init(&result, stack);
	char buf[MOVE_STRING_MAX_LENGTH] = { '\0' };
	move_to_string(result.best_move, buf);
	fprintf(engine->config.output, "bestmove %s\n", buf);
}

void
engine_start_search(struct Engine *engine)
{
	struct SStack stack = sstack_new(engine);
	while (true) {
		struct SStackPlieIter *last_plie = sstack_last(&stack);
		// We use depth-first search (DFS) to explore the game tree.
		if (!plieiter_has_next(&last_plie->iter)) {
			if (stack.plie_i == 0) {
				break;
			} else {
				sstack_pop(&stack);
			}
		} else if (stack.plie_i == stack.desired_depth) {
			ssplieiter_eval_leaf(last_plie, &stack.board);
		} else {
			sstack_push(&stack);
		}
	}
	finish_search(engine, &stack);
	sstack_delete(&stack);
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
