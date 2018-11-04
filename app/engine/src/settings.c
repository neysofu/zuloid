#include "settings.h"
#include "chess/color.h"
#include "clock.h"
#include "globals.h"
#include "switches.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
settings_default(struct Settings *settings)
{
	assert(settings);
	settings->debug = false;
	settings->ponder = true;
	settings->train = false;
	settings->use_time_control = true;
	settings->port = DEFAULT_PORT;
	settings->max_cache_size = SWITCH_MAX_CACHE_SIZE;
	settings->move_selection_noise = 0.005;
	settings->resoluteness = 0.5;
	settings->seed = DEFAULT_SEED;
	settings->selectivity = 0.005;
	settings->resign_rate = 0.08;
	settings->time_control[0] = clock_new_blitz();
	settings->time_control[1] = clock_new_blitz();
	settings->max_depth = 0;
	settings->max_num_nodes = 0;
	settings->buffer = xmalloc(20);
};

char *
settings_set_value(struct Settings *settings, char *name, char *value)
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
		case 0x6e87002190572330: // "seed"
			settings->seed = atoi(value);
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
