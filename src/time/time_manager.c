/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "time/game_clock.h"
#include "time/time_control.h"
#include "utils.h"

msec_time
game_clock_estimate_thinking_time(struct GameClock *game_clock, int moves_count)
{
	/* TODO: doesn't take into account overtime, increments and delays. */
	return game_clock->time_left / moves_count;
}
