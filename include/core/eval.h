/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CORE_CPU_EVALUATION_H
#define Z64C_CORE_CPU_EVALUATION_H

#include "chess/move.h"
#include "chess/position.h"
#include "core/agent.h"
#include <stdint.h>
#include <stdlib.h>

struct Eval
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
