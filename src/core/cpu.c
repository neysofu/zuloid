/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "agent.h"
#include "cJSON/cJSON.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "engine.h"
#include "eval.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
buffer_init_from_position(int64_t buf[], struct Position *pos)
{
	File ep_file = square_file(pos->en_passant_target);
	int ep_feature = ep_file == FILE_NONE ? 0 : 1 << ep_file;
	buf[0] = pos->bb[COLOR_WHITE];
	buf[1] = pos->bb[COLOR_BLACK];
	/* The input features must perfectly locate each piece. Maybe 8*8 features per piece,
	 * showing where it is. I absolutely want to reduce the search space to a minimum.
	 *
	 * But then I also to reuse board evaluation...  How do I do that? Basically I should
	 * engineer the network so that it always first evaluates the board, then it applies a
	 * move. The move can also be null though.
	 *
	 * White king (8*2)
	 * Black king (8*2)
	 * Pawns? I think bitboards are fine. It should be simple
	 * */
	buf[2] = pos->bb[PIECE_TYPE_PAWN];
	buf[3] = pos->bb[PIECE_TYPE_KNIGHT];
	buf[4] = pos->bb[PIECE_TYPE_KING];
	buf[5] = pos->bb[PIECE_TYPE_ROOK];
	buf[6] = pos->bb[PIECE_TYPE_BISHOP];
	if (pos->reversible_moves_count < 50) {
		buf[10] = (1 << pos->reversible_moves_count) - 1;
	} else {
		buf[10] = ~0;
	}
	buf[11] = pos->side_to_move | (color_other(pos->side_to_move) << 1) |
	          (pos->castling_rights << 2) | (ep_feature << 6);
}

size_t
buffer_deserialize_from_json(int64_t buf[], cJSON *obj)
{
	/* TODO */
	cJSON *obj_buf_width = cJSON_GetObjectItem(obj, "width");
	cJSON *obj_buf_data = cJSON_GetObjectItem(obj, "data");
	assert(obj_buf_width);
	assert(obj_buf_data);
	size_t buf_width = obj_buf_width->valuedouble;
	char *buf_data = obj_buf_data->valuestring;
	return b64_decode(buf_data, strlen(buf_data), buf) / sizeof(int64_t);
}

cJSON *
buffer_serialize_into_json(int64_t buf[], size_t width)
{
	char *buf_data = malloc(b64e_size(buf, width * sizeof(int64_t)));
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
	AGENT_COMPRESSED_WIDTH = 128,

	AGENT_L0_WIDTH = 1024,
	AGENT_L0_COMPRESSED_WIDTH = 16,

	AGENT_L1_WIDTH = 4096,
	AGENT_L1_COMPRESSED_WIDTH = 64,

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

/* TODO: Training via simple error backpropagation. Then I should try a very simple network
 * that rewards capture.
 *
 * Neuron model: each neuron takes in inputs from the previous layer (most likely through a
 * filter) and outputs a single bit via population count threshold.
 *
 * The output layer has no bitboards. That would require much more learning for no advantage
 * at all. Also, the output layer should NOT be in bits but rather in bytes.
 * - Piece placement.
 * - Floating point indicators. */
struct Agent
{
	int64_t layer_alpha[AGENT_COMPRESSED_WIDTH];
	int64_t layer_bravo[AGENT_COMPRESSED_WIDTH];
	int64_t buffer[AGENT_COMPRESSED_WIDTH];
	int64_t weights_0_1[AGENT_L1_WIDTH][AGENT_L0_COMPRESSED_WIDTH];
	/* Real-valued outputs. */
	bool move_features[AGENT_MOVE_FEATURES_COUNT];
	float indicators[AGENT_INDICATORS_COUNT];
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		memset(agent, 0, sizeof(struct Agent));
		agent_randomly_init_weights(agent);
	}
	return agent;
}

void
agent_randomly_init_weights(struct Agent *agent)
{

	for (size_t i = 0; i < AGENT_L1_WIDTH; i++) {
		for (size_t j = 0; j < AGENT_L0_COMPRESSED_WIDTH; j++) {
			agent->weights_0_1[i][j] = genrand64_int64();
		}
	}
}

void
agent_feedforward(struct Agent *agent,
                  struct Engine *engine,
                  int64_t *weights,
                  size_t width_in,
                  size_t width_out)
{
	size_t out_i = 0;
	uint64_t out_mask = 1;
	for (size_t i = 0; i < width_out * 64; i++) {
		ENGINE_DEBUGF(engine, "[TRACE] Now feeding neuron (%zu).\n", i);
		/* We now collect all incoming signals to this neuron. */
		for (size_t j = 0; j < width_in; j++) {
			/* Brutally parallel solution. TODO: explore Four Russian's method and
			 * similar optimizations? */
			agent->buffer[j] = agent->layer_alpha[j] & weights[i * j];
		}
		/* The activation point is fixed at half the number of incoming connections. */
		size_t popcount = popcnt(agent->buffer, width_in * sizeof(int64_t));
		ENGINE_DEBUGF(engine,
		              "[TRACE] The population count for the (%zu)th neuron is (%zu).\n",
		              i,
		              popcount);
		if (popcount > width_in * 32) {
			agent->layer_bravo[out_i] |= out_mask;
			ENGINE_DEBUGF(engine, "[TRACE] This neuron will activate.\n");
		} else {
			ENGINE_DEBUGF(engine, "[TRACE] This neuron won't activate.\n");
		}
		if (!(out_mask <<= 1)) {
			out_mask = 0x1ULL << 63;
			out_i++;
		}
	}
}

void
engine_start_search(struct Engine *engine)
{
	ENGINE_DEBUGF(engine, "[INFO] Now searching...\n");
	/* The first step is to evaluate the position with the neural network. */
	buffer_init_from_position(&engine->agent->layer_alpha, &engine->position);
	agent_feedforward(engine->agent,
	                  engine,
	                  &engine->agent->weights_0_1[0][0],
	                  AGENT_L0_WIDTH,
	                  AGENT_L1_WIDTH);
	float centipawns = engine->agent->indicators[INDICATOR_COLOR_WHITE] -
	                   engine->agent->indicators[INDICATOR_COLOR_BLACK];
	/* Now pick any of the suggested moves. In Z64C there is no best move found in the
	 * evaluation, only candidate moves. Moves are suggested by  square: */
	char buf[6] = { '\0' };
	struct Move moves[MAX_MOVES] = { 0 };
	size_t count = gen_legal_moves(moves, &engine->position);
	move_to_string(moves[genrand64_int64() % count], buf);
	/* TODO: Hide print behind protocol-specific logic. */
	printf("info score cp %f\n", centipawns);
	printf("bestmove %s\n", buf);
}

void
engine_stop_search(struct Engine *engine)
{
	assert(engine);
	ENGINE_DEBUGF(engine, "[INFO] Interrupting search.\n");
}

void
agent_delete(struct Agent *agent)
{
	free(agent);
}
