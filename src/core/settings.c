#include <stdlib.h>
#include <stdbool.h>
#include "settings.h"

uint64_t Z64C_CACHE_MAX_SIZE = 1024 * 1024 * 1024;
uint32_t Z64C_REDIS_PORT = 6379;
uint16_t Z64C_ANN_BATCH_SIZE = 16;
char *Z64C_PATH_TO_REDIS_CONF = "";
bool Z64C_TRAINING = false;
char *Z64C_PATH_TO_TENSORFLOW_MODEL = "";

char *Z64C_TEST_DIR = "";

void
settings_init(void) {
	char *env_var = getenv("Z64C_CACHE_MAX_SIZE");
	if (env_var) {
		Z64C_CACHE_MAX_SIZE = atoi(env_var);
	}
	/*if (isalpha(s[])) {
		switch (c) {
			case 'k':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 10;
				break;
			case 'm':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 20;
				break;
			case 'g':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 30;
				break;
			case 't':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 40;
				break;
			case 'p':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 50;
				break;
			default:
				break;
		}
		switch (c) {
			case 'B':
				Z64C_TRANSPOS_TABLE_MAX_SIZE <<= 3;
				break;
			default:
				break;
		}
	}*/
	env_var = getenv("Z64C_REDIS_PORT");
	if (env_var) {
		Z64C_REDIS_PORT = atoi(env_var);
	}
	env_var = getenv("Z64C_ANN_BATCH_SIZE");
	if (env_var) {
		Z64C_ANN_BATCH_SIZE = atoi(env_var);
	}
	env_var = getenv("Z64C_PATH_TO_REDIS_CONF");
	if (env_var) {
		Z64C_PATH_TO_REDIS_CONF = env_var;
	}
	env_var = getenv("Z64C_TRAINING");
	if (env_var) {
		Z64C_TRAINING = atoi(env_var);
	}
}
