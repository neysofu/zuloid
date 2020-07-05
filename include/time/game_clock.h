#ifndef ZORRO_TIME_GAME_CLOCK_H
#define ZORRO_TIME_GAME_CLOCK_H

#include "time/time_control.h"
#include <plibsys.h>
#include <stdint.h>

struct GameClock
{
	float time_left_in_seconds;
	PTimeProfiler *timer;
	int moves_count;
	struct TimeControl *time_control;
};

void
game_clock_init(struct GameClock *gc, const struct TimeControl *tc);

float
game_clock_start(struct GameClock *gc);

void
game_clock_stop(struct GameClock *gc);

#endif
