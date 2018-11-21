/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file settings.h
 * @brief Settings that change the default behavior of the program.
 */

#pragma once

#include "clock.h"
#include <stdbool.h>
#include <stdlib.h>

/// @brief A grobal settings manager modifiable at runtime.
struct Settings
{
	bool debug;
	/// When set to 1, the engine will think during opponent's time.
	bool ponder;
	/// If set, the command @a train will become available.
	bool train;
	bool use_time_control;
	/// The port reserved for network features such as clustering.
	uint32_t port;
	uint32_t seed;
	/// The maximum size of `struct TTable` instances in bytes.
	size_t max_cache_size;
	/// Random noise between 0 and 1 that is added to candidade moves' scores to
	/// improve randomness.
	float move_selection_noise;
	/// @brief The minimum probability of winning before Z64C resigns.
	/// @note Resigning is not available with the @a train command.
	float resign_rate;
	/// @brief A measure of preference for winning over drawing.
	/// A resoluteness score of 0.0 means that Z64C will make no distincion
	/// between drawing and winning and will treat both outcomes equally. On the
	/// contrary, a resoluteness score of 1.0 will treat draws as losses.
	///
	/// The default value is 0.5.
	float resoluteness;
	/// @brief A measure of the approach. Low selectivity scores will favour
	/// shallow searching but exaustive searching; a high selectivity score will
	/// favour deep and sharp playing lines.
	///
	/// The default and optimal value is 0.5.
	float selectivity;
	/// @brief The maximum number of half moves the engine is allowed to look
	/// ahead during search.
	size_t max_depth;
	/// @brief The maximum number of chess positions the engine is allowed to
	/// evaluate.
	size_t max_num_nodes;
	struct Tablebase *tablebase;
};

/// @brief Reset the settings manager @p settings to its default value.
/// @pre @p settings is not `NULL`.
void
settings_default(struct Settings *settings);

/// @brief Updates a certain setting with the intended value @p value. Returns
/// NULL if the setting is successfully updated, an error message otherwise.
/// @return @p field iff the operation was successful; `NULL` otherwise. Reasons
/// of failure can be:
/// - nonexistant name, or
/// - illegal value.
int8_t
settings_set_value(struct Settings *settings, char *name, char *value);