/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_SETTINGS_H
#define Z64C_SETTINGS_H

#include <stdint.h>
#include <stdlib.h>

struct Settings
{
	/* This is the network port in use for engine clusters. */
	uint_least32_t port;
	/* The random seed. */
	uint_least32_t seed;
	/* Random noise between 0 and 1 that is added to candidade moves' scores to
	 * improve randomness. */
	float move_selection_noise;
	/* The contempt score favors a risky, high-reward kind of play. It is
	 * included in the range [0;1]. */
	float contempt;
	/* The higher the selectivity score, the lower the branching factor during
	 * search is. It must be in the range [0;1]. */
	float selectivity;
	struct Tablebase *tablebase;
};

extern const struct Settings SETTINGS_DEFAULT;

#endif
