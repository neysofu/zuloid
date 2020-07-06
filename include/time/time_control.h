#ifndef ZORRO_TIME_TIME_CONTROL_H
#define ZORRO_TIME_TIME_CONTROL_H

#include <stdint.h>

// A unilateral (i.e. for one player only) time control with support for increments,
// delays, and transition to a different time control after a certain amount of
// moves. All timestamps have millisecond precision.
//
// # On latency
// The `time_control` structure does *not* take latency into consideration. The
// client should keep time_controls in sync by sending updates with timestamps.
//
// # Resources
// - https://en.wikipedia.org/wiki/Time_control
// - https://www.chessprogramming.org/Time_Management#Time_Controls
struct TimeControl
{
	float time_limit_in_seconds;
	float increment_in_seconds;
	float delay_in_seconds;
	int max_moves_count;
	struct TimeControl *next_time_control;
	struct TimeControl *previous_time_control;
	struct TimeControl *overtime;
};

// Handy initializers for three standard FIDE time controls:
// - bullet (1+0),
// - blitz (3+2),
// - rapid (15+10), and
// - classical (90+30 plus 30 extra minutes after move 40).
struct TimeControl *
time_control_new_bullet(void);

void
time_control_delete(struct TimeControl *tc);

#endif
