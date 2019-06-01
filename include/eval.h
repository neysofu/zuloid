/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_H
#define Z64C_EVAL_H

#include "chess/move.h"
#include "chess/position.h"
#include <stdint.h>
#include <stdlib.h>

/* The evaluation report that Z64C generates after it's done searching. */
struct Eval
{
	float game_phase_indicator;
	float dispersion;
	/* Centipawns score. */
	float cp;
	/* Number of legal moves in the current position. */
	size_t moves_count;
	struct Move *moves;
};

#endif
