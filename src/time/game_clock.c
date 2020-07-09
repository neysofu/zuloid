#include "time/game_clock.h"
#include "time/time_control.h"
#include <plibsys.h>
#include <time.h>
#include <stdlib.h>

void
game_clock_init(struct GameClock *gc, struct TimeControl *tc)
{
	*gc = (struct GameClock) {
		.time_left_in_seconds = tc->time_limit_in_seconds,
		.timer = p_time_profiler_new(),
		.moves_count = 0,
		.time_control = tc,
	};
}

float
game_clock_start_explicit(struct GameClock *gc)
{
	float time_left_in_seconds = gc->time_left_in_seconds;
	struct TimeControl *overtime = gc->time_control->overtime;
	while (overtime) {
		time_left_in_seconds += overtime->time_limit_in_seconds;
		overtime = overtime->overtime;
	}
	return time_left_in_seconds;
}

float
game_clock_start(struct GameClock *gc)
{
	p_time_profiler_reset(gc->timer);
	return game_clock_start_explicit(gc);
}

void
game_clock_stop_explicit(struct GameClock *gc, float time_elapsed_in_seconds)
{
	gc->time_left_in_seconds -= time_elapsed_in_seconds;
	gc->time_left_in_seconds += gc->time_control->increment_in_seconds;
	gc->moves_count++;
	if (gc->moves_count >= gc->time_control->max_moves_count) {
		gc->time_control = gc->time_control->next_time_control;
		gc->time_left_in_seconds += gc->time_control->time_limit_in_seconds;
	}
	if (gc->time_left_in_seconds < 0) {
		gc->time_control = gc->time_control->overtime;
	}
}

void
game_clock_stop(struct GameClock *gc)
{
	game_clock_stop_explicit(gc, (float)p_time_profiler_elapsed_usecs(gc->timer) * 1000 * 1000);
}
