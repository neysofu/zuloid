/**
 * @file ttable_node.h
 * @brief Search nodes.
 */

#pragma once

#include "chess/board.h"
#include <stdint.h>

struct TTableNode
{
	// uint16_t frequencies[SWITCH_NUM_AVG_CANDIDATE_MOVES];
	struct Board board;
	uint64_t hash;
	struct TTableNode *lnode;
	struct TTableNode *rnode;
};
