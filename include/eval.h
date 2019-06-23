/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

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
