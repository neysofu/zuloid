#ifndef ZULOID_CONFIG_H
#define ZULOID_CONFIG_H

#include <stdbool.h>
#include <stdlib.h>

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

#endif
