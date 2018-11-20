/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "settings.h"
#include "chess/color.h"
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
	settings->port = DEFAULT_PORT;
	settings->max_cache_size = SWITCH_MAX_CACHE_SIZE;
	settings->move_selection_noise = 0.005;
	settings->resoluteness = 0.5;
	settings->seed = DEFAULT_SEED;
	settings->selectivity = 0.005;
	settings->resign_rate = 0.08;
	settings->max_depth = 0;
	settings->max_num_nodes = 0;
};

int8_t
settings_set_value(struct Settings *settings, char *name, char *value)
{
	assert(settings);
	assert(name);
	assert(value);
	switch (XXH64(name, strlen(name), 0)) {
		case 0x01fd51a2a6f9cc2f: /** "debug" */
			settings->debug = atof(value);
			break;
		case 0x0a6f394a3987568a: /** "ponder" */
			settings->ponder = atof(value);
			break;
		case 0xd18f9b6611eb8e16: /** "train" */
			settings->train = atof(value);
			break;
		case 0x6e87002190572330: /** "seed" */
			settings->seed = atoi(value);
			break;
		case 0x1fa9e1c58f24f03b: /** "debug" */
			settings->debug = atoi(value);
			break;
		case 0x498733265ad1a88e: /** "max-cache-size" */
			settings->max_cache_size = atoi(value);
			break;
		case 0x866f5b204efc0f22: /** "move-selection-noise" */
			settings->move_selection_noise = atof(value);
			break;
		case 0xf291148a28ff30db: /** "resign-rate" */
			settings->resign_rate = atof(value);
			break;
		case 0x20e46ecc0c1029f5: /** "resoluteness" */
			settings->resoluteness = atof(value);
			break;
		case 0x92cfb5ce943dee0d: /** "selectivity" */
			settings->selectivity = atof(value);
			break;
		default:
			return -1;
	}
	return 0;
}
