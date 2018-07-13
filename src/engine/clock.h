#include <stdint.h>

struct Clock {
	uint32_t white_msec;
	uint32_t black_msec;
	uint32_t white_increment_msec;
	uint32_t black_increment_msec;
	uint32_t white_delay_msec;
	uint32_t black_delay_msec;
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
