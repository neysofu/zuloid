/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_TIME_GAME_CLOCK_H
#define Z64C_TIME_GAME_CLOCK_H

#include "tictoc/tictoc.h"
#include "time/time_control.h"
#include <stdint.h>

struct GameClock
{
	float time_left_in_seconds;
	TicTocTimer timer;
	int moves_count;
	struct TimeControl *time_control;
};

struct GameClock *
game_clock_new(struct TimeControl *tc);

void
game_clock_delete(struct GameClock *gc);

float
game_clock_start(struct GameClock *gc);

void
game_clock_stop(struct GameClock *gc);

#endif
