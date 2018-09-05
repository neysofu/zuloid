#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "color.h"
#include "settings.h"
#include "utils.h"

struct Settings
settings_default(void) {
	struct Settings settings = {
		.debug = false,
		.ponder = true,
		.train = false,
		.use_time_control = true,
		.port = SWITCH_PORT,
		.max_cache_size_in_bytes = SWITCH_MAX_CACHE_SIZE_IN_BYTES,
		.move_selection_noise = 0.005,
		.resign_rate = 0.08,
		.time_control = {
			clock_new_blitz(),
			clock_new_blitz(),
		},
		.max_depth = 0,
		.max_num_nodes = 0,
	};
	return settings;
};

void
settings_print(struct Settings *settings) {
	printf("Debug: %s\n",
		   settings->debug ? "yes" : "no");
	printf("Think during opponent's time: %s\n",
		   settings->ponder ? "yes" : "no");
	printf("Train heuristics during play: %s\n",
		   settings->train ? "yes" : "no");
	printf("Use time control: %s\n",
		   settings->use_time_control ? "yes" : "no");
	printf("Network port in use: %lu\n",
		   settings->port);
	printf("Maximum size of the cache table (bytes): %s\n",
		   settings->max_cache_size_in_bytes);
	printf("Noise applied during move selection: %f\n",
		   settings->move_selection_noise);
	printf("Resign rate: %f\n",
		   settings->resign_rate);
	printf("White's thinking time (msec): %d\n",
		   settings->time_control[COLOR_WHITE]->time_available_msec);
	printf("White's increment (msec): %d\n",
		   settings->time_control[COLOR_WHITE]->time_increment_msec);
	printf("White's Bronstein delay (msec): %d\n",
		   settings->time_control[COLOR_WHITE]->time_delay_msec);
	printf("Black's thinking time (msec): %d\n",
		   settings->time_control[COLOR_BLACK]->time_available_msec);
	printf("Black's increment (msec): %d\n",
		   settings->time_control[COLOR_BLACK]->time_increment_msec);
	printf("Black's Bronstein delay (msec): %d\n",
		   settings->time_control[COLOR_BLACK]->time_delay_msec);
	printf("Maximum depth (plies): %d\n",
		   settings->max_depth);
	printf("Maximum num. of nodes: %llu\n",
		   settings->max_num_nodes);
}
