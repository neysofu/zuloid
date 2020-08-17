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

