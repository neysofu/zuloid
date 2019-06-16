/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * A neural network that operates on bitboards. */

#include "agent.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "engine.h"
#include "eval.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
	AGENT_BUFFER_WIDTH = 128,
	AGENT_W0_WIDTH = 16,
	AGENT_W1_WIDTH = 32,
};

struct Agent
{
	/* Holds the input layer's data. */
	int64_t layer_in[AGENT_BUFFER_WIDTH];
	/* Holds the output layer's data. */
	int64_t layer_out[AGENT_BUFFER_WIDTH];
	/* Useful computation buffer. */
	int64_t buffer[AGENT_BUFFER_WIDTH];
	int64_t w_0_1[AGENT_W0_WIDTH][AGENT_W1_WIDTH];
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		/* Initialize weights with random data... FIXME, of course. */
		for (size_t i = 0; i < AGENT_W0_WIDTH; i++) {
			for (size_t j = 0; j < AGENT_W1_WIDTH; j++) {
				srand(0x517f);
				agent->w_0_1[i][j] = rand();
			}
		}
	}
	return agent;
}

void
agent_init_position(struct Agent *agent, struct Position *pos)
{
	agent->layer_in[0] = pos->bb[COLOR_WHITE];
	agent->layer_in[1] = pos->bb[COLOR_BLACK];
	agent->layer_in[2] = pos->bb[PIECE_TYPE_PAWN];
	agent->layer_in[3] = pos->bb[PIECE_TYPE_KNIGHT];
	agent->layer_in[4] = pos->bb[PIECE_TYPE_KING];
	agent->layer_in[5] = pos->bb[PIECE_TYPE_ROOK];
	agent->layer_in[6] = pos->bb[PIECE_TYPE_BISHOP];
	agent->layer_in[7] = BB_LIGHT_SQUARES;
	agent->layer_in[8] = BB_DARK_SQUARES;
	agent->layer_in[9] = BB_CENTER_SQUARES;
	if (pos->reversible_moves_count < 50) {
		agent->layer_in[10] = (1 << pos->reversible_moves_count) - 1;
	} else {
		agent->layer_in[10] = -1;
	}
	agent->layer_in[11] = pos->side_to_move | (pos->castling_rights << 1);
}

void
agent_eval(struct Agent *restrict agent)
{
	for (size_t i = 0; i < AGENT_W1_WIDTH; i++) {
		for (size_t j = 0; j < AGENT_W0_WIDTH; j++) {
			/* Brutally parallel solution. TODO: explore Four Russian's method and similar
			 * optimizations? */
			agent->buffer[j] = agent->layer_in[j] ^ agent->w_0_1[i][j];
		}
		/* Reiterate and convert agent->buffer into agent->layer_out... */
		/* FIXME! */
		for (size_t j = 0; j < AGENT_W0_WIDTH; j++) {
			int64_t mask = 0x1ULL << 63;
			if (__builtin_popcount(agent->buffer[j]) > 32) {
				agent->layer_out[j / 64] |= mask;
			}
			if (!(mask >>= 1)) {
				mask = 0x1ULL << 63;
			}
		}
	}
}

void
engine_start_search(struct Engine *engine)
{
	assert(engine);
	ENGINE_DEBUGF(engine, "[INFO] Now searching...\n");
	agent_init_position(engine->agent, &engine->position);
	size_t w_score = popcnt(engine->agent->layer_out + COLOR_WHITE, sizeof(int64_t));
	size_t b_score = popcnt(engine->agent->layer_out + COLOR_BLACK, sizeof(int64_t));
	float centipawns = w_score - b_score;
	printf("info score cp %f\n", centipawns);
	agent_eval(engine->agent);
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
