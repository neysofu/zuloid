/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "time/game_clock.h"
#include "tictoc/tictoc.h"
#include "time/time_control.h"
#include "utils.h"
#include <stdlib.h>

void
game_clock_init(struct GameClock *gc, const struct TimeControl *tc)
{
	gc->time_left_in_seconds = tc->time_limit_in_seconds;
	gc->timer = tic();
	gc->moves_count = 0;
	gc->time_control = tc;
}

float
game_clock_start(struct GameClock *gc)
{
	gc->timer = tic();
	float time_left_in_seconds = gc->time_left_in_seconds;
	struct TimeControl *overtime = gc->time_control->overtime;
	while (overtime) {
		time_left_in_seconds += overtime->time_limit_in_seconds;
		overtime = overtime->overtime;
	}
	return time_left_in_seconds;
}

void
game_clock_stop(struct GameClock *gc)
{
	gc->time_left_in_seconds -= toc(&gc->timer);
	gc->time_left_in_seconds += gc->time_control->increment_in_seconds;
	gc->moves_count++;
	if (gc->time_control->max_moves_count == gc->moves_count) {
		gc->time_control = gc->time_control->next_time_control;
		gc->time_left_in_seconds += gc->time_control->time_limit_in_seconds;
	}
	if (gc->time_left_in_seconds < 0) {
		gc->time_control = gc->time_control->overtime;
	}
}
