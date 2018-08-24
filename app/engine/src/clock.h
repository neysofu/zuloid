#pragma once

#include <stdbool.h>
#include <stdint.h>

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

// Create 90+30 time control for the first 40 moves, then adds 30 minutes.
struct Clock *
clock_classical(struct Clock *clock);

// Create a 15+10 time control.
struct Clock *
clock_rapid(struct Clock *clock);

// Create a 3+2 time control.
struct Clock *
clock_blitz(struct Clock *clock);

// Free a clock and all its related time controls.
void
clock_drop(struct Clock *clock);

// Start the clock and return the timestamp in milliseconds after which it will
// flag.
uint64_t
clock_start(struct Clock *clock);

// Stop a ticking clock and check if time expired. '1' means there is some time
// left, '0' means it's over.
bool
clock_stop(struct Clock *clock);
