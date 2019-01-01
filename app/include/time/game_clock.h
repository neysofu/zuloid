/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_TIME_GAME_CLOCK_H
#define Z64C_TIME_GAME_CLOCK_H

#include "time/time_control.h"
#include <stdint.h>

struct GameClock
{
	msec_time time_left;
	msec_time last_trigger;
	uint_least16_t trigger_count;
	struct TimeControl *time_control;
};

struct GameClock *
game_clock_new(struct TimeControl *tc);

void
game_clock_delete(struct GameClock *gc);

msec_time
game_clock_start(struct GameClock *gc);

void
game_clock_stop(struct GameClock *gc);

#endif
