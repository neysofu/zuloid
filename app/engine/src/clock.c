#include <stdlib.h>
#include "clock.h"

struct Clock *
clock_new(struct Clock *clock) {
	if (!clock) {
		clock = malloc(sizeof(struct Clock));
		if (!clock) {
			return NULL;
		}
	}
	return clock;
}

void
clock_drop(struct Clock *clock) {
	free(clock);
}

void
clock_start(struct Clock *clock) {
	clock->expiry_time; // TODO
}

void
clock_stop(struct Clock *clock) {
	clock->expiry_time += clock->increment_msec;
}
