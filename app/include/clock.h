/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Time controls for chess. */

#pragma once

#include <stdlib.h>

/* A unilateral (i.e. for one player only) clock with support for increments,
 * delays, and transition to a different time control after a certain amount of
 * moves. All timestamps have millisecond precision.
 *
 * On latency
 *   The `clock` structure does *not* take latency into consideration. The
 *   client should keep clocks in sync by sending updates with timestamps.
 * Resources
 *   https://en.wikipedia.org/wiki/Time_control
 *   https://www.chessprogramming.org/Time_Management#Time_Controls */
struct Clock
{
	/* Timestamp keeping track of the last time that this clock was started. */
	uint64_t start_timestamp_msec;
	/* Time reserve can expire and the count will then be negative. */
	int64_t time_available_msec;
	/* Traditional per-move increment. */
	uint64_t increment_msec;
	/* Bronstein delay. */
	uint64_t delay_msec;
	/* Support for multistage time controls. The trigger event is move n.`trigger_move`. */
	struct Clock *next_stage;
	struct Clock *previous_stage;
	uint16_t trigger_move_num;
	/* After main time is expired, this optional time control will kick in. It
	 * will kick in as soon as the current time is finished.
	 *
	 * I'm aware of many unorthodox time controls and this overtime method
	 * supports quite a few of them, including byo-yomi and Canadian overtime. */
	struct Clock *overtime;
};

/* These functions create simple clocks with common FIDE time controls. */
struct Clock *
clock_new_blitz(void);
struct Clock *
clock_new_rapid(void);
struct Clock *
clock_new_classical(void);

/* Frees a `clock`. */
void
clock_free(struct Clock *clock);

// Starts the clock and returns the timestamp in milliseconds after which it
// will flag.
uint64_t
clock_start(struct Clock *clock);

/* Stops the ticking of `clock` and checks for flagging.
 *
 * Result
 *   0 on success and -1 on failure. */
int8_t
clock_stop(struct Clock *clock);
