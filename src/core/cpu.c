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

typedef int64_t Cell;

enum
{
	AGENT_BUF_IN_WIDTH = 16,
	AGENT_BUF_OUT_WIDTH = 48,

	AGENT_MAX_BUF_WIDTH = 48,
};

struct Agent
{
	int64_t buf[AGENT_MAX_BUF_WIDTH];
	int64_t l_in[AGENT_BUF_IN_WIDTH];
	int64_t l_0[AGENT_BUF_IN_WIDTH][AGENT_BUF_OUT_WIDTH];
	int64_t l_out[AGENT_BUF_OUT_WIDTH];
	/* Output information. */
	Bitboard piece_maps[16][COLORS_COUNT];
	int64_t scores[COLORS_COUNT];
	int64_t game_phase_indicator;
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	/* TODO */
	return agent;
}

int
agent_import(struct Agent *agent, FILE *file)
{
	fseek(file, 0, SEEK_END);
	size_t file_length = ftell(file);
	char *buffer = malloc(file_length);
	// FIXME: malloc check
	fread(buffer, 1, file_length, file);
	fclose(file);
	cJSON *json = cJSON_Parse(buffer);
	// FIXME: malloc check
	return 0;
}

void
agent_init_position(struct Agent *agent, struct Position *pos)
{
	agent->buf[0] = pos->bb[COLOR_WHITE];
	agent->buf[1] = pos->bb[COLOR_BLACK];
	agent->buf[2] = pos->bb[PIECE_TYPE_PAWN];
	agent->buf[3] = pos->bb[PIECE_TYPE_KNIGHT];
	agent->buf[4] = pos->bb[PIECE_TYPE_KING];
	agent->buf[5] = pos->bb[PIECE_TYPE_ROOK];
	agent->buf[6] = pos->bb[PIECE_TYPE_BISHOP];
	agent->buf[7] = BB_LIGHT_SQUARES;
	agent->buf[8] = BB_DARK_SQUARES;
	agent->buf[9] = BB_CENTER_SQUARES;
	if (pos->reversible_moves_count < 50) {
		agent->buf[10] = (1 << pos->reversible_moves_count) - 1;
	} else {
		agent->buf[10] = -1;
	}
	agent->buf[11] = pos->side_to_move | (pos->castling_rights << 1);
}

void
agent_eval(struct Agent *restrict agent)
{
	// for (size_t i = 0; i < AGENT_BUF_IN_WIDTH; i++) {
	//	for (size_t j = 0; j < AGENT_BUF_OUT_WIDTH; j++) {
	//		agent->buf_out[j] |= agent->buf_in[i] & agent->l0[i * j][0];
	//		agent->buf_out[j] |= agent->buf_in[i] ^ agent->l0[i * j][1];
	//		agent->buf_out[j] ^= agent->buf_in[i] & agent->l0[i * j][2];
	//	}
	//}
}

void
engine_start_search(struct Engine *engine)
{
	assert(engine);
	ENGINE_DEBUGF(engine, "[INFO] Now searching...\n");
	agent_init_position(engine->agent, &engine->position);
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
