/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Settings that change the default behavior of the program. */

#pragma once

#include "cJSON/cJSON.h"
#include "clock.h"
#include <stdbool.h>
#include <stdlib.h>

/* A global settings manager modifiable at runtime. */
struct Settings
{
	/* This is the network port in use for engine clusters. */
	uint32_t port;
	/* The random seed. */
	uint32_t seed;
	/* The maximum amount of memory allocated for the transpositions table in
	 * bytes. */
	size_t max_cache_size;
	/// Random noise between 0 and 1 that is added to candidade moves' scores to
	/// improve randomness.
	float move_selection_noise;
	/* Once the estimated winning chances fall below this threshold, Z64C will
	 * resign. */
	float resign_rate;
	/* The resoluteness score favors a risky, high-reward kind of play. It is
	 * included in the range [0;1]. */
	float resoluteness;
	/* The higher the selectivity score, the lower the branching factor during
	 * search is. It must be in the range [0;1]. */
	float selectivity;
	struct Tablebase *tablebase;
};

/* Resets the settings manager `settings` back to default values.
 *
 * Preconditions
 *   The parameter `settings` must be non-NULL. */
void
settings_default(struct Settings *settings);

/* Changes a specific field of the `settings` instance.
 *
 * Parameters
 *   - `settings`: a non-NULL pointer to the settings manager.
 *   - `name`: the name of the setting that shall be updated with the given
 *     value.
 *   - `value`: The JSON encoded value that one wishes to assign to `name`. It
 *     can be any kind of JSON value (most of the times a number), but
 *     it must be of the expected type for the given setting `name`. E.g., the
 *     `port` setting only supports numbers and null values.
 * Result
 *   0 on success. On failure to update the setting, any of these error codes:
 *   - -1: the given setting `name` is not supported.
 *   - -2: the JSON `value` that was provided is not meaningful for the `name`
 *   setting.
 * Preconditions
 *   All `settings`, `name`, and `value` parameters are non-NULL pointers.
 */
int8_t
settings_set_value(struct Settings *settings, const char *name, const struct cJSON *value);
