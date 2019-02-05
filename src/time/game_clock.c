/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "time/game_clock.h"
#include "time/time_control.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

static msec_time
current_msec_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

struct GameClock *
game_clock_new(struct TimeControl *tc)
{
	struct GameClock *gc = malloc_or_exit(sizeof(struct GameClock));
	gc->time_left = tc->time_limit;
	gc->last_trigger = 0;
	gc->trigger_count = 0;
	gc->time_control = tc;
	return gc;
}

void
game_clock_delete(struct GameClock *gc)
{
	if (!gc) {
		return;
	}
	free(gc->time_control);
	free(gc);
}

msec_time
game_clock_start(struct GameClock *gc)
{
	assert(gc);
	gc->last_trigger = current_msec_time();
	return gc->last_trigger + gc->time_left;
}

void
game_clock_stop(struct GameClock *gc)
{
	assert(gc);
	gc->time_left -= (current_msec_time() - gc->last_trigger);
	gc->time_left += gc->time_control->increment;
	gc->trigger_count++;
	if (gc->time_control->max_trigger_count == gc->trigger_count) {
		gc->time_control = gc->time_control->next_time_control;
		gc->time_left += gc->time_control->time_limit;
	}
	if (gc->time_left < 0) {
		gc->time_control = gc->time_control->overtime;
	}
}
