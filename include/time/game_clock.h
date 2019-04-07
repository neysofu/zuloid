/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_TIME_GAME_CLOCK_H
#define Z64C_TIME_GAME_CLOCK_H

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
