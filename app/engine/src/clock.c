#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include "clock.h"
#include "utils.h"

void
clock_drop(struct Clock *clock) {
	if (!clock) {
		return;
	}
	// Recursion is fine here since we'll go down 1 or 2 levels max.
	clock_drop(clock->next_clock);
	clock_drop(clock->previous_clock);
	free(clock);
}

uint64_t
clock_start(struct Clock *clock) {
	clock->start_timestamp_msec = timestamp_msec();
	return clock->start_timestamp_msec + clock->time_available_msec;
}

bool
clock_stop(struct Clock *clock) {
	uint64_t time_elapsed_msec = timestamp_msec() - clock->start_timestamp_msec;
	clock->time_available_msec -= time_elapsed_msec;
	if (clock->time_available_msec < 0) {
		return false;
	}
	clock->time_available_msec += clock->time_increment_msec;
	clock->time_available_msec += MIN(clock->time_delay_msec, time_elapsed_msec);
	return true;
}

struct Clock *
clock_classical(struct Clock *clock) {
	if (!clock) {
		clock = malloc(sizeof(struct Clock));
	}
	struct Clock *clock_after_40 = malloc(sizeof(struct Clock));
	clock->start_timestamp_msec = 0;
	clock->time_available_msec = 90 * 60 * 1000;
	clock->time_increment_msec = 30 * 1000;
	clock->time_delay_msec = 0;
	clock->next_clock_trigger_num_move = 40;
	clock->next_clock = clock_after_40;
	clock->previous_clock = NULL;
	clock_after_40->start_timestamp_msec = 0;
	clock_after_40->time_available_msec = 30 * 60 * 1000;
	clock_after_40->time_increment_msec = 30 * 1000;
	clock_after_40->time_delay_msec = 0;
	clock_after_40->next_clock_trigger_num_move = 40;
	clock_after_40->next_clock = NULL;
	clock_after_40->previous_clock = clock;
	return clock;
}

struct Clock *
clock_rapid(struct Clock *clock) {
	if (!clock) {
		clock = malloc(sizeof(struct Clock));
	}
	clock->start_timestamp_msec = 0;
	clock->time_available_msec = 15 * 60 * 1000;
	clock->time_increment_msec = 10 * 1000;
	clock->time_delay_msec = 0;
	clock->next_clock_trigger_num_move = 0;
	clock->next_clock = NULL;
	clock->previous_clock = NULL;
	return clock;
}

struct Clock *
clock_blitz(struct Clock *clock) {
	if (!clock) {
		clock = malloc(sizeof(struct Clock));
	}
	clock->start_timestamp_msec = 0;
	clock->time_available_msec = 3 * 60 * 1000;
	clock->time_increment_msec = 2 * 1000;
	clock->time_delay_msec = 0;
	clock->next_clock_trigger_num_move = 0;
	clock->next_clock = NULL;
	clock->previous_clock = NULL;
	return clock;
};
