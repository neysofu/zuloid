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
	settings->port = DEFAULT_PORT;
	settings->move_selection_noise = 0.005;
	settings->resoluteness = 0.5;
	settings->seed = DEFAULT_SEED;
	settings->selectivity = 0.005;
	settings->resign_rate = 0.08;
};

int8_t
settings_set_value(struct Settings *settings, const char *name, const struct cJSON *value)
{
	assert(settings);
	assert(name);
	assert(value);
	switch (XXH64(name, strlen(name), 0)) {
		case 0x6e87002190572330: /* "seed" */
			settings->seed = atoi(value);
			break;
		case 0x866f5b204efc0f22: /* "move-selection-noise" */
			settings->move_selection_noise = value->valuedouble;
			break;
		case 0xf291148a28ff30db: /* "resign-rate" */
			settings->resign_rate = value->valuedouble;
			break;
		case 0x20e46ecc0c1029f5: /* "resoluteness" */
			settings->resoluteness = value->valuedouble;
			break;
		case 0x92cfb5ce943dee0d: /* "selectivity" */
			settings->selectivity = value->valuedouble;
			break;
		default:
			return -1;
	}
	return 0;
}
