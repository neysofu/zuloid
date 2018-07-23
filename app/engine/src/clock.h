#include <stdint.h>

struct Clock {
	uint32_t expiry_time;
	uint32_t increment_msec;
	uint32_t delay_msec;
	uint8_t full_move_next_clock;
	struct Clock *next_clock;
};

struct Clock *
clock_new(struct Clock *clock);

void
clock_drop(struct Clock *clock);

void
clock_start(struct Clock *clock);

void
clock_stop(struct Clock *clock);
