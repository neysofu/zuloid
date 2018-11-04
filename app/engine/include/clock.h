/**
 * @file clock.h
 * @brief Time controls for chess.
 */

#pragma once

#include <stdlib.h>

/**
 * A unilateral (i.e. for one player only) clock with support for increments,
 * delays, and transition to a different time control after a certain amount of
 * moves.
 */
struct Clock
{
	uint64_t start_timestamp_msec;
	int64_t time_available_msec;
	uint64_t time_increment_msec;
	uint64_t time_delay_msec;
	uint64_t time_lag_msec;
	uint16_t num_moves_before_next_stage;
	struct Clock *next_stage;
	struct Clock *previous_stage;
};

struct Clock *
clock_new_blitz(void);
struct Clock *
clock_new_rapid(void);
struct Clock *
clock_new_classical(void);

// Frees a clock.
void
clock_free(struct Clock *clock);

// Starts the clock and returns the timestamp in milliseconds after which it
// will flag.
uint64_t
clock_start(struct Clock *clock);

/**
 * @brief Stop the ticking of @p clock and returns an errors code, if any.
 * @return 0 if there if time left on the clock, else -1.
 */
int8_t
clock_stop(struct Clock *clock);
