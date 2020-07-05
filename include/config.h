#pragma once

#include "agent.h"
#include "cache/cache.h"
#include "chess/position.h"
#include "chess/termination.h"
#include "eval.h"
#include "time/game_clock.h"
#include <stdio.h>

struct Config
{
	bool debug;
	float move_selection_noise;
	float contempt;
	float selectivity;
	bool ponder;
	size_t max_nodes_count;
	size_t max_depth;
};
