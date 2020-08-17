#include "agent.h"
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

size_t
buffer_deserialize_from_json(int64_t buf[], cJSON *obj)
{
	/* TODO */
	cJSON *obj_buf_width = cJSON_GetObjectItem(obj, "width");
	cJSON *obj_buf_data = cJSON_GetObjectItem(obj, "data");
	assert(obj_buf_width);
	assert(obj_buf_data);
	char *buf_data = obj_buf_data->valuestring;
	return b64_decode(buf_data, strlen(buf_data), buf) / sizeof(int64_t);
}

cJSON *
buffer_serialize_into_json(int64_t buf[], size_t width)
{
	char *buf_data = malloc(b64e_size(width * sizeof(int64_t)));
	if (!buf_data) {
		return NULL;
	}
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddNumberToObject(obj, "width", width);
	cJSON_AddStringToObject(obj, "data", buf_data);
	return obj;
}

enum
{
	AGENT_BUF_WIDTH = 128,

	AGENT_L0_WIDTH = 1024,
	AGENT_L0_COMPRESSED_WIDTH = 16,

	AGENT_L1_WIDTH = 4096,
	AGENT_L1_COMPRESSED_WIDTH = 64,

	AGENT_L2_WIDTH = 320,
	AGENT_L2_COMPRESSED_WIDTH = 5,

	AGENT_PAWN_MOVE_FEATURES_COUNT = 8 * 4,
	AGENT_KNIGHT_MOVE_FEATURES_COUNT = 2 * 8,
	AGENT_BISHOP_MOVE_FEATURES_COUNT = 3 * 14,
	AGENT_ROOK_MOVE_FEATURES_COUNT = 3 * 14,
	AGENT_KING_MOVE_FEATURES_COUNT = 1 * 10,
	AGENT_MOVE_FEATURES_COUNT =
	  AGENT_PAWN_MOVE_FEATURES_COUNT + AGENT_KNIGHT_MOVE_FEATURES_COUNT +
	  AGENT_BISHOP_MOVE_FEATURES_COUNT + AGENT_ROOK_MOVE_FEATURES_COUNT +
	  AGENT_KING_MOVE_FEATURES_COUNT,

	AGENT_INDICATORS_COUNT = 5,
	INDICATOR_COLOR_WHITE = 0,
	INDICATOR_COLOR_BLACK = 1,
	INDICATOR_EVALUATION = 2,
	INDICATOR_SHARPNESS = 3,
};

///* TODO: Training via simple error backpropagation. Then I should try a very
/// simple
/// network
// * that rewards capture.
// *
// * Neuron model: each neuron takes in inputs from the previous layer (most
// likely through
// a
// * filter) and outputs a single bit via population count threshold - then some
// are inverted.
// *
// * The output layer has no bitboards. That would require much more learning
// for no advantage
// * at all. Also, the output layer should NOT be in bits but rather in bytes.
// * - Piece placement.
// * - Floating point indicators.
// *
// *
// * Board2Vec:
// * Builds a through understanding of chess rules by the network by creating a
// neural
// * embedding of boards. The input layer is a bitboard-based binarized encoding
// of the board.
// * */
struct Agent
{
	int64_t buf_popcnt[AGENT_BUF_WIDTH];
	/* Input and output layers by rotation. */
	int64_t buf_alpha[AGENT_BUF_WIDTH];
	int64_t buf_bravo[AGENT_BUF_WIDTH];
	/* Layers. */
	int64_t filters_0[AGENT_L0_COMPRESSED_WIDTH];
	int64_t weights_0_1[AGENT_L1_WIDTH][AGENT_L0_COMPRESSED_WIDTH];
	int32_t thresholds_1[AGENT_L1_WIDTH];
	int64_t inversions_1[AGENT_L1_COMPRESSED_WIDTH];
};
//
// void
// agent_randomly_init_heuristics(struct Agent *agent)
//{
//	for (size_t i = 0; i < AGENT_L1_WIDTH; i++) {
//		for (size_t j = 0; j < AGENT_L0_COMPRESSED_WIDTH; j++) {
//			agent->weights_0_1[i][j] = genrand64_int64();
//		}
//	}
//	for (size_t i = 0; i < AGENT_L2_WIDTH; i++) {
//		agent->biases_2[i] = genrand64_int64() & 0xffff;
//		for (size_t j = 0; j < AGENT_L1_COMPRESSED_WIDTH; j++) {
//			agent->weights_1_2[i][j] = genrand64_int64();
//		}
//	}
//}
//
struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		memset(agent, 0, sizeof(struct Agent));
		// agent_randomly_init_heuristics(agent);
	}
	return agent;
}
//
// void
// agent_predict(struct Agent *agent, struct Board *pos)
//{
//	/* Input layer evaluation. */
//	agent->buf_popcnt[0] = pos->bb[0];
//	size_t out_i = 0;
//	uint64_t out_mask = 1;
//	for (size_t i = 0; i < AGENT_L1_WIDTH; i++) {
//		for (size_t j = 0; j < AGENT_L0_COMPRESSED_WIDTH; j++) {
//			agent->buf_popcnt[j] = agent->layer_alpha[j] &
// agent->weights_0_1[i][j];
//		}
//		/* The activation point is fixed at half the number of incoming
// connections. */ 		size_t popcount = 		  popcnt(agent->buffer,
// AGENT_L1_COMPRESSED_WIDTH * sizeof(int64_t)); 		if (popcount > AGENT_L0_WIDTH /
// 4) { 			agent->layer_bravo[out_i] |= out_mask; 			ENGINE_LOGF(engine,
// "[TRACE] This neuron will activate.\n"); 		} else { 			ENGINE_LOGF(engine,
// "[TRACE] This neuron won't activate.\n");
//		}
//		if (!(out_mask <<= 1)) {
//			out_mask = 0x1ULL << 63;
//			out_i++;
//		}
//	}
//}

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
		printf("new eval is %f\n", -eval);
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
