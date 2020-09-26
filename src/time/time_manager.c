/* SPDX-License-Identifier: GPL-3.0-only */

#include "time/game_clock.h"
#include "time/time_control.h"

float
game_clock_estimate_thinking_time_in_seconds(struct GameClock *game_clock, int moves_count)
{
	/* TODO: doesn't take into account overtime, increments and delays. */
	return game_clock->time_left_in_seconds / moves_count;
}
