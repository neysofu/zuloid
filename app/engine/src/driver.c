#include "driver.h"
#include "api.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/result.h"
#include "clock.h"
#include "cmd.h"
#include "game.h"
#include "globals.h"
#include "log.h"
#include "mode.h"
#include "mt19937-64/mt64.h"
#include "search/ttable.h"
#include "settings.h"
#include "switches.h"
#include "utils.h"
#include "xxHash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sysexits.h>
#include <unistd.h>

struct Driver*
driver_new(void)
{
	struct Driver* driver = xmalloc(sizeof(struct Driver));
	driver->ttable = ttable_new();
	driver->board = BOARD_STARTPOS;
	driver->result = RESULT_NONE;
	driver->game = game_new();
	driver->mode = MODE_IDLE;
	settings_default(&driver->settings);
	driver->exit_status = EX_OK;
	return driver;
}

void
driver_free(struct Driver* driver)
{
	if (!driver) {
		debug_printf("Freeing a NULL pointer.\n");
		return;
	}
	ttable_free(driver->ttable);
	game_free(driver->game);
	free(driver);
}

void
driver_seed(struct Driver* driver)
{
	assert(driver);
	driver->seed = genrand64_int64();
}

int8_t
driver_main(struct Driver* driver)
{
	assert(driver);
	struct Cmd* cmd = cmd_new();
	char* str = NULL;
	size_t str_length = 0;
	int8_t exit_status = EX_OK;
	while (driver->mode != MODE_QUIT) {
		if (getline(&str, &str_length, stdin) == -1) {
			return EX_IOERR;
		}
		exit_status = driver_run_str(driver, str);
	}
	return exit_status;
}

int8_t
driver_run_str(struct Driver* driver, char* str)
{
	assert(driver);
	assert(str);
	struct Cmd* cmd = cmd_new();
	int8_t exit_status = cmd_parse(cmd, str);
	if (exit_status != 0) {
		return exit_status;
	}
	char* cmd_arg_0 = cmd_arg_ith(cmd, 0);
	if (!cmd_arg_0 || cmd_arg_0[0] == '#') {
		return EX_USAGE;
	}
	switch (XXH64(cmd_arg_0, strlen(cmd_arg_0), 0)) {
		case 0x33bf00a859c4ba3f: // "foo"
			return driver_call_foo(driver, cmd);
		case 0x2682377a1193c238: // "get"
			return driver_call_get(driver, cmd);
		case 0x757dfc0052b6ac66: // "load"
			return driver_call_load(driver, cmd);
		case 0xec22b9453ad51a5a: // "move"
			return driver_call_move(driver, cmd);
		case 0x707db5f765aed6d8: // "quit"
			return driver_call_quit(driver, cmd);
		case 0xd795d67f2e9e6996: // "save"
			return driver_call_save(driver, cmd);
		case 0x71e6f6d1e157dbfe:
			return driver_call_search(driver, cmd);
		case 0x24115d14507a61eb: // "set"
			return driver_call_set(driver, cmd);
		case 0xde2128284d8fdb3c: // "setup"
			return driver_call_setup(driver, cmd);
#if Z64C_SWITCH_TRAIN
		case 0xd18f9b6611eb8e16:
			return driver_call_train(driver, cmd);
#endif
		default:
			fprintf(
			  stderr, "error \"'%s' is not a known UCI command.\"\n", cmd_arg_0);
			return EX_OK;
	}
}
