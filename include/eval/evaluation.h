/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_EVALUATION_H
#define Z64C_EVAL_EVALUATION_H

#include "chess/move.h"
#include <stdint.h>
#include <stdlib.h>

struct Evaluation
{
	float game_phase_indicator;
	float dispersion;
	float centipawns;
	size_t moves_count;
	struct
	{
		Square source;
		Square target;
	} * sliding_moves;
	bool castling_oo;
	bool castling_ooo;
	bool en_passant;
};

#endif
