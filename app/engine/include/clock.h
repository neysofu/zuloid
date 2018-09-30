/// @file clock.h
/// @brief Time controls for chess.

#pragma once

#include <stdlib.h>

// A unilateral (i.e. for one player only) clock with support for increments,
// delays, and transition to a different time control after a certain amount of
// moves.
struct Clock {
	uint64_t start_timestamp_msec;
	int64_t time_available_msec;
	uint64_t time_increment_msec;
	uint64_t time_delay_msec;
	uint32_t next_clock_trigger_num_move;
	struct Clock *next_clock;
	struct Clock *previous_clock;
};

// Set up a clock with FIDE blitz time control.
struct Clock *
clock_new_blitz(void);

// Set up a clock with FIDE rapid time control.
struct Clock *
clock_new_rapid(void);

// Set up a clock with FIDE classical time control.
struct Clock *
clock_new_classical(void);

// Frees a clock.
void
clock_drop(struct Clock *clock);

// Starts the clock and returns the timestamp in milliseconds after which it will
// flag.
uint64_t
clock_start(struct Clock *clock);

// Stops a ticking clock and returns the timestamp in milliseconds after which
// it flagged or will flag.
uint64_t
clock_stop(struct Clock *clock);
