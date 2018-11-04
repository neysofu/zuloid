/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <assert.h>
#include <stdlib.h>
#include "chess/coord.h"
#include "chess/dir.h"

enum Dir
dir_from_to(Coord coord_0, Coord coord_1) {
	assert(coord_is_in_bounds(coord_0));
	assert(coord_is_in_bounds(coord_1));
	uint_fast8_t horizontal_shift = abs(coord_file(coord_0) -
									    coord_file(coord_1));
	uint_fast8_t vertical_shift = abs(coord_rank(coord_0) -
									  coord_rank(coord_1));
	if (horizontal_shift == 0) {
		return DIR_VERTICAL;
	} else if (vertical_shift == 0) {
		return DIR_HORIZONTAL;
	} else if (horizontal_shift == vertical_shift) {
		return DIR_DIAGONAL;
	} else if (horizontal_shift + vertical_shift == 3) {
		return DIR_L_SHAPED;
	} else {
		return DIR_NONE;
	}
}
