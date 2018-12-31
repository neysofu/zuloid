/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_TIME_TIME_CONTROL_H
#define Z64C_TIME_TIME_CONTROL_H

#include <stdio.h>
#include <stdlib.h>

typedef int_least64_t msec_time;

/* A unilateral (i.e. for one player only) time control with support for increments,
 * delays, and transition to a different time control after a certain amount of
 * moves. All timestamps have millisecond precision.
 *
 * On latency
 *   The `time_control` structure does *not* take latency into consideration. The
 *   client should keep time_controls in sync by sending updates with timestamps.
 * Resources
 *   https://en.wikipedia.org/wiki/Time_control
 *   https://www.chessprogramming.org/Time_Management#Time_Controls */
struct TimeControl
{
	msec_time time_limit;
	msec_time increment;
	msec_time delay;
	uint_least16_t max_trigger_count;
	/* Support for multistage time controls as commonly seen in tournaments. The trigger
	 * event is move n.`trigger_move`. */
	struct TimeControl *next_time_control;
	struct TimeControl *previous_time_control;
	/* After main time is expired, this optional time control will kick in. It
	 * will kick in as soon as the current time is finished.
	 *
	 * I'm aware of many unorthodox time controls and this overtime method
	 * supports quite a few of them, including byo-yomi and Canadian overtime. */
	struct TimeControl *overtime;
};

/* Handy initializers for three standard FIDE time controls:
 * - blitz (3+2),
 * - rapid (15+10), and
 * - classical (90+30 plus 30 extra minutes after move 40). */
struct TimeControl *
time_control_new_blitz(void);
struct TimeControl *
time_control_new_rapid(void);
struct TimeControl *
time_control_new_classical(void);

void
time_control_delete(struct TimeControl *tc);

#endif
