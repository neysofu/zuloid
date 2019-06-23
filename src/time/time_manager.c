/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "time/game_clock.h"
#include "time/time_control.h"

float
game_clock_estimate_thinking_time_in_seconds(struct GameClock *game_clock, int moves_count)
{
	/* TODO: doesn't take into account overtime, increments and delays. */
	return game_clock->time_left_in_seconds / moves_count;
}
