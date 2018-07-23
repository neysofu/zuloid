#pragma once

#include <stdbool.h>
#include <stdint.h>

void
settings_init(void);

extern uint64_t Z64C_CACHE_MAX_SIZE_IN_BYTES;
extern uint32_t Z64C_REDIS_PORT;
extern char *Z64C_PATH_TO_REDIS_CONF;
extern uint16_t Z64C_PORT;
extern bool Z64C_TRAINING;
extern char *Z64C_DIR;
extern char *Z64C_TEST_DIR;
extern float Z64C_MOVE_SELECTION_NOISE;
