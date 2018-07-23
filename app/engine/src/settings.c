#include <stdlib.h>
#include <stdbool.h>
#include "settings.h"
#include "utils.h"

uint64_t Z64C_CACHE_MAX_SIZE_IN_BYTES = 128 * 1024 * 1024; // 128MB
uint32_t Z64C_REDIS_PORT = 5640;
char *Z64C_PATH_TO_REDIS_CONF = "";
bool Z64C_TRAINING = false;
char *Z64C_PATH_TO_TENSORFLOW_MODEL = "";
char *Z64C_TEST_DIR = "";
float Z64C_MOVE_SELECTION_NOISE = 0.02;

void
settings_init(void) {
	char *env_var = getenv("Z64C_CACHE_MAX_SIZE");
	if (env_var) {
		Z64C_CACHE_MAX_SIZE_IN_BYTES = atoi(env_var);
	}
	env_var = getenv("Z64C_REDIS_PORT");
	if (env_var) {
		Z64C_REDIS_PORT = atoi(env_var);
	}
	env_var = getenv("Z64C_PATH_TO_REDIS_CONF");
	if (env_var) {
		Z64C_PATH_TO_REDIS_CONF = env_var;
	}
	env_var = getenv("Z64C_TRAINING");
	if (env_var) {
		Z64C_TRAINING = atob(env_var);
	}
	env_var = getenv("Z64C_MOVE_SELECTION_NOISE");
	if (env_var) {
		Z64C_MOVE_SELECTION_NOISE = atof(env_var);
	}
	env_var = getenv("Z64C_TEST_DIR");
	if (env_var) {
		Z64C_TEST_DIR = env_var;
	}
}
