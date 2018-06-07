#pragma once

#include "move.h"
#include "uci.h"

struct SearchParams {
	bool ponder;
	uint64_t white_time;
	uint64_t black_time;
	uint64_t white_increment;
	uint64_t black_increment;
	uint16_t moves_to_go;
	uint16_t depth;
	size_t num_nodes;
	uint16_t mate_length;
	size_t time;
	bool infinite;
	struct Move moves[];
};

struct SearchParams *
uci_cmd_to_search_params(struct UciCmd *cmd, struct SearchParams *params);
