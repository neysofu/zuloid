/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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

Cell
cell_from_reversible_moves_count(size_t reversible_moves_count)
{
	return (1 << reversible_moves_count) - 1;
}

Cell
cell_from_active_color(enum Color color)
{
	Cell cell = color;
	cell |= color_other(color) << 1;
	return cell;
}

/* A layer is a trainable transformation from one cell cluster to another. */
struct Layer
{
	size_t count_in;
	size_t count_out;
	Cell *cells;
};

Cell *
layer_or_cells(struct Layer *layer)
{
	return layer->cells;
}

Cell *
layer_and_cells(struct Layer *layer)
{
	return layer->cells + layer->count_in * layer->count_out;
}

Cell *
layer_xor_cells(struct Layer *layer)
{
	return layer->cells + layer->count_in * layer->count_out * 2;
}

void
cells_init_from_position(Cell cells[], struct Position *position)
{
	assert(cells);
	assert(position);
	cells[0] = position->bb[COLOR_WHITE];
	cells[1] = position->bb[COLOR_BLACK];
	cells[2] = position->bb[PIECE_TYPE_PAWN];
	cells[3] = position->bb[PIECE_TYPE_KNIGHT];
	cells[4] = position->bb[PIECE_TYPE_KING];
	cells[5] = position->bb[PIECE_TYPE_ROOK];
	cells[6] = position->bb[PIECE_TYPE_BISHOP];
	cells[9] = cell_from_active_color(position->side_to_move);
	cells[10] = cell_from_reversible_moves_count(position->reversible_moves_count);
	cells[11] = position->castling_rights;
}

void
layer_transform(struct Layer *layer, Cell in[], Cell out[])
{
	assert(layer);
	assert(in);
	assert(out);
	Cell *xor_cells = layer_xor_cells(layer);
	Cell *or_cells = layer_or_cells(layer);
	Cell *and_cells = layer_and_cells(layer);
	for (size_t i = 0; i < layer->count_in; i++) {
		for (size_t j = 0; j < layer->count_out; i++) {
			/* FIXME */
			out[j] = in[i] & and_cells[i * j];
			out[j] = in[i] & or_cells[i * j];
			out[j] = in[i] & xor_cells[i * j];
		}
	}
}

struct Network
{
	struct Layer l0;
};

void
network_run(struct Network *network, struct Position *position, Cell buffer[])
{
	Cell in[128] = { 0 };
	cells_init_from_position(in, position);
	layer_transform(&network->l0, in, buffer);
}

struct Agent
{
	FILE *source;
	struct Network network;
	struct Eval eval;
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		*agent = (struct Agent){
			.source = NULL,
		};
	}
	return agent;
}

void
agent_delete(struct Agent *agent)
{
	free(agent);
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

int
agent_export(struct Agent *agent, FILE *file)
{
	fclose(file);
	return 0;
}

int
agent_eval_position(struct Agent *agent, struct Position *position)
{
	/* N and M dimension tensors, I need to convert one into the other. Great.
	 * I want a SUPER simple interface... I could use bitshifts for changing
	 * places. So I would need N bitshifts to also influence other bits. How
	 * the hell do I do that? Basically, if there are more than N signals, it's
	 * ok. N is chosen to keep the number of activated neurons stable through
	 * the layers.
	 * to focus on output neurons one at a time.
	 * 1. Count all firing connections to neuron. If it's enough, then keep it. */
	return 0;
}

void
engine_start_search(struct Engine *engine)
{
	assert(engine);
}

void
engine_stop_search(struct Engine *engine)
{
	assert(engine);
}
