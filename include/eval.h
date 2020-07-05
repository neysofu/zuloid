#ifndef ZORRO_EVAL_H
#define ZORRO_EVAL_H

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
