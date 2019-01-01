/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "settings.h"

const struct Settings SETTINGS_DEFAULT = {
	.port = 34290,
	.seed = 0xcfca130bUL,
	.cache_block_size = SIZE_MAX,
	.max_cache_blocks_count = SIZE_MAX,
	.move_selection_noise = 0.005,
	.contempt = 0.5,
	.selectivity = 0.5,
};
