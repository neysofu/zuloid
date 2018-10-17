#include "settings.h"
#include "chess/color.h"
#include "clock.h"
#include "switches.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
settings_default(struct Settings* settings)
{
	assert(settings);
	settings->debug = false;
	settings->ponder = true;
	settings->train = false;
	settings->use_time_control = true;
	settings->port = SWITCH_PORT;
	settings->max_cache_size = SWITCH_MAX_CACHE_SIZE;
	settings->move_selection_noise = 0.005;
	settings->resoluteness = 0.5;
	settings->selectivity = 0.005;
	settings->resign_rate = 0.08;
	settings->time_control[0] = clock_new_blitz();
	settings->time_control[1] = clock_new_blitz();
	settings->max_depth = 0;
	settings->max_num_nodes = 0;
	settings->buffer = xmalloc(20);
};

void
settings_print(struct Settings* settings)
{
	assert(settings);
	printf("debug: %d\n", settings->debug);
	printf("ponder: %d\n", settings->ponder);
	printf("train: %d\n", settings->train);
	printf("use-time-control: %d\n", settings->use_time_control);
	printf("port: %d\n", settings->port);
	printf("maximum-cache-size: %zu\n", settings->max_cache_size);
	printf("move-selection-noise: %.3f\n", settings->move_selection_noise);
	printf("resign-rate: %.3f\n", settings->resign_rate);
	// printf("white-time-control",
	//	   settings->time_control[COLOR_WHITE]->time_available_msec);
	// printf("White's increment (msec): %d\n",
	//	   settings->time_control[COLOR_WHITE]->time_increment_msec);
	// printf("White's Bronstein delay (msec): %d\n",
	//	   settings->time_control[COLOR_WHITE]->time_delay_msec);
	// printf("Black's thinking time (msec): %d\n",
	//	   settings->time_control[COLOR_BLACK]->time_available_msec);
	// printf("Black's increment (msec): %d\n",
	//	   settings->time_control[COLOR_BLACK]->time_increment_msec);
	// printf("Black's Bronstein delay (msec): %d\n",
	//	   settings->time_control[COLOR_BLACK]->time_delay_msec);
	printf("max-depth: %zu\n", settings->max_depth);
	printf("max-num-nodes: %zu\n", settings->max_num_nodes);
}

char*
settings_value(struct Settings* settings, char* name)
{
	assert(settings);
	assert(name);
	size_t i;
	for (i = 0; name[i]; i++) {
		name[i] = tolower(name[i]);
	}
	switch (XXH64(name, i, 0)) {
		case 0x01fd51a2a6f9cc2f:
			sprintf(settings->buffer, "%d", settings->debug);
			break;
		case 0x0a6f394a3987568a:
			sprintf(settings->buffer, "%d", settings->ponder);
			break;
		case 0xd18f9b6611eb8e16:
			sprintf(settings->buffer, "%d", settings->train);
			break;
		case 0x74e00972f2209886: // use-time-control
			// TODO
			break;
		case 0x1fa9e1c58f24f03b:
			sprintf(settings->buffer, "%d", settings->debug);
			break;
		case 0x498733265ad1a88e:
			sprintf(settings->buffer, "%zu", settings->max_cache_size);
			// TODO: units (GiB, ecc.).
			break;
		case 0x866f5b204efc0f22:
			sprintf(settings->buffer, "%.3f", settings->move_selection_noise);
			break;
		case 0xf291148a28ff30db:
			sprintf(settings->buffer, "%.3f", settings->resign_rate);
			break;
		case 0x20e46ecc0c1029f5:
			sprintf(settings->buffer, "%.3f", settings->resoluteness);
			break;
		case 0x92cfb5ce943dee0d:
			sprintf(settings->buffer, "%.3f", settings->selectivity);
			break;
		default:
			return "Invalid field.";
	}
	return settings->buffer;
}

char*
settings_set_value(struct Settings* settings, char* name, char* value)
{
	assert(settings);
	assert(name);
	assert(value);
	size_t i;
	for (i = 0; name[i]; i++) {
		name[i] = tolower(name[i]);
	}
	switch (XXH64(name, i, 0)) {
		case 0x01fd51a2a6f9cc2f: // "debug"
			settings->debug = atof(value);
			break;
		case 0x0a6f394a3987568a: // "ponder"
			settings->ponder = atof(value);
			break;
		case 0xd18f9b6611eb8e16: // "train"
			settings->train = atof(value);
			break;
		case 0x74e00972f2209886: // use-time-control
			settings->use_time_control = atoi(value);
			break;
		case 0x1fa9e1c58f24f03b: // "debug"
			settings->debug = atoi(value);
			break;
		case 0x498733265ad1a88e: // "max-cache-size"
			settings->max_cache_size = atoi(value);
			break;
		case 0x866f5b204efc0f22: // "move-selection-noise"
			settings->move_selection_noise = atof(value);
			break;
		case 0xf291148a28ff30db: // "resign-rate"
			settings->resign_rate = atof(value);
			break;
		case 0x20e46ecc0c1029f5: // "resoluteness"
			settings->resoluteness = atof(value);
			break;
		case 0x92cfb5ce943dee0d: // "selectivity"
			settings->selectivity = atof(value);
			break;
		default:
			return "Invalid field.";
	}
	return NULL;
}
