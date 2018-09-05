#pragma once

#include <stdbool.h>
#include <stdlib.h>

// A unilateral (i.e. for one player only) clock with support for increments,
// delays, and transition to a different time control after a certain amount of
// moves. Please note that the first move's increment is only added after the
// first move and not before, contrary to what FIDE rules impose.
struct Clock {
	uint64_t start_timestamp_msec;
	int64_t time_available_msec;
	uint64_t time_increment_msec;
	uint64_t time_delay_msec;
	uint32_t next_clock_trigger_num_move;
	struct Clock *next_clock;
	struct Clock *previous_clock;
};

struct Clock *
clock_new_blitz(void);

struct Clock *
clock_new_rapid(void);

struct Clock *
clock_new_classical(void);

// Frees any memory allocated for 'clock'.
void
clock_drop(struct Clock *clock);

// Starts the clock and returns the timestamp in milliseconds after which it will
// flag.
uint64_t
clock_start(struct Clock *clock);

// Stops a ticking clock and checks if time expired. 'true' means there is some
// time left, 'false' means it's over.
bool
clock_stop(struct Clock *clock);
