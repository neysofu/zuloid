#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "utils.h"

struct Settings
settings_default(void) {
	return (struct Settings) {
		.debug = false,
		.ponder = true,
		.train = false,
		.use_clocks = false,
		.port = 24210,
		.max_cache_size_in_bytes = 1E+9,
		.move_selection_noise = 0.005,
		.resign_rate = 0.08,
		.clocks = {
			CLOCK_FIDE_BLITZ,
			CLOCK_FIDE_BLITZ,
		},
		.max_depth = 0,
		.max_num_nodes = 0,
	};
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
		   settings->use_clocks ? "yes" : "no");
	printf("Network port in use: %lu\n",
		   settings->port);
	printf("Maximum size of the cache table (bytes): %s\n",
		   settings->max_cache_size_in_bytes);
	printf("Noise applied during move selection: %f\n",
		   settings->move_selection_noise);
	printf("Resign rate: %f\n",
		   settings->resign_rate);
	printf("White's thinking time (msec): %d\n",
		   settings->clocks[COLOR_WHITE]->time_available_msec);
	printf("White's increment (msec): %d\n",
		   settings->clocks[COLOR_WHITE]->time_increment_msec);
	printf("White's Bronstein delay (msec): %d\n",
		   settings->clocks[COLOR_WHITE]->time_delay_msec);
	printf("Black's thinking time (msec): %d\n",
		   settings->clocks[COLOR_BLACK]->time_available_msec);
	printf("Black's increment (msec): %d\n",
		   settings->clocks[COLOR_BLACK]->time_increment_msec);
	printf("Black's Bronstein delay (msec): %d\n",
		   settings->clocks[COLOR_BLACK]->time_delay_msec);
	printf("Maximum depth (plies): %d\n",
		   settings->max_depth);
	printf("Maximum num. of nodes: %llu\n",
		   settings->max_num_nodes);
}
