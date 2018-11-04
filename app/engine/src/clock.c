#include "clock.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

struct Clock *
clock_new_blitz(void)
{
	struct Clock *clock = xmalloc(sizeof(struct Clock));
	clock->start_timestamp_msec = 0;
	clock->time_available_msec = 3 * 60 * 1000;
	clock->time_increment_msec = 2 * 1000;
	clock->time_delay_msec = 0;
	clock->time_lag_msec = 0;
	clock->num_moves_before_next_stage = 0;
	clock->next_stage = NULL;
	clock->previous_stage = NULL;
	return clock;
}

struct Clock *
clock_new_rapid(void)
{
	struct Clock *clock = xmalloc(sizeof(struct Clock));
	clock->start_timestamp_msec = 0;
	clock->time_available_msec = 15 * 60 * 1000;
	clock->time_increment_msec = 10 * 1000;
	clock->time_delay_msec = 0;
	clock->time_lag_msec = 0;
	clock->num_moves_before_next_stage = 0;
	clock->next_stage = NULL;
	clock->previous_stage = NULL;
	return clock;
}

struct Clock *
clock_new_classical(void)
{
	struct Clock *clock_0 = xmalloc(sizeof(struct Clock));
	struct Clock *clock_1 = xmalloc(sizeof(struct Clock));
	clock_0->start_timestamp_msec = 0;
	clock_0->time_available_msec = 90 * 60 * 1000;
	clock_0->time_increment_msec = 30 * 1000;
	clock_0->time_delay_msec = 0;
	clock_0->time_lag_msec = 0;
	clock_0->num_moves_before_next_stage = 40;
	clock_0->next_stage = clock_1;
	clock_0->previous_stage = NULL;
	clock_1->start_timestamp_msec = 0;
	clock_1->time_available_msec = 30 * 60 * 1000;
	clock_1->time_increment_msec = 30 * 1000;
	clock_1->time_lag_msec = 0;
	clock_1->time_delay_msec = 0;
	clock_1->num_moves_before_next_stage = 0;
	clock_1->next_stage = NULL;
	clock_1->previous_stage = clock_0;
	return clock_0;
}

void
clock_free(struct Clock *clock)
{
	assert(clock);
	assert(!clock->previous_stage);
	while (clock->previous_stage) {
		clock = clock->previous_stage;
	}
	while (clock->next_stage) {
		clock = clock->next_stage;
		free(clock->previous_stage);
	}
	free(clock);
}

uint64_t
clock_start(struct Clock *clock)
{
	clock->start_timestamp_msec = util_timestamp_msec();
	return clock->start_timestamp_msec + clock->time_available_msec;
}

int8_t
clock_stop(struct Clock *clock)
{
	uint64_t time_elapsed_msec =
	  util_timestamp_msec() - clock->start_timestamp_msec;
	clock->time_available_msec -= time_elapsed_msec;
	if (clock->time_available_msec < 0) {
		return -1;
	} else {
		clock->time_available_msec +=
		  clock->time_increment_msec +
		  MIN(clock->time_delay_msec, time_elapsed_msec);
		return 0;
	}
}
