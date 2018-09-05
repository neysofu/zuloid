#pragma once

#include "clock.h"

// Settings modifiable at runtime with UCI commands or with "ucirc".
struct Settings {
	bool debug;
	bool ponder;
	bool train;
	bool use_time_control;
	uint32_t port;
	uint64_t max_cache_size_in_bytes;
	float move_selection_noise;
	float resign_rate;
	struct Clock *time_control[2];
	uint32_t max_depth;
	uint64_t max_num_nodes;
};

// Provide defaults designed for commodity hardware.
struct Settings
settings_default(void);

// Print to stdout a report of current setting values.
void
settings_print(struct Settings *settings);
