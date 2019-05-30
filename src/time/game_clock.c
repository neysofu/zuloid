/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "time/game_clock.h"
#include "time/time_control.h"
#include <assert.h>
#include <plibsys.h>
#include <stdlib.h>

void
game_clock_init(struct GameClock *gc, const struct TimeControl *tc)
{
	assert(gc);
	assert(tc);
	gc->time_left_in_seconds = tc->time_limit_in_seconds;
	gc->timer = p_time_profiler_new();
	gc->moves_count = 0;
	gc->time_control = (struct TimeControl *)(tc);
}

float
game_clock_start(struct GameClock *gc)
{
	p_time_profiler_reset(gc->timer);
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
	gc->time_left_in_seconds -= p_time_profiler_elapsed_usecs(gc->timer) * 1000 * 1000;
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
