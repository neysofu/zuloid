#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>
#include <sysexits.h>
#include <unistd.h>
#include "api.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/result.h"
#include "clock.h"
#include "cmd.h"
#include "driver.h"
#include "game.h"
#include "globals.h"
#include "mode.h"
#include "settings.h"
#include "switches.h"
#include "search/ttable.h"
#include "utils.h"
#include "xxHash.h"

struct Driver *
driver_new(void) {
	struct Driver *driver = malloc(sizeof(struct Driver));
	if (!driver) {
		return NULL;
	}
	if (!(driver->ttable = ttable_new())) {
		goto drop_after_ttable;
	}
	driver->board = BOARD_STARTPOS;
	driver->result = RESULT_NONE;
	if (!(driver->game = game_new())) {
		goto drop_after_game;
	}
	driver->mode = MODE_IDLE;
	settings_default(&driver->settings);
	driver->exit_status = EX_OK;
	return driver;
drop_after_game:
	ttable_drop(driver->ttable);
drop_after_ttable:
	free(driver);
	return NULL;
}

void
driver_drop(struct Driver *driver) {
	if (!driver) {
		return;
	}
	ttable_drop(driver->ttable);
	game_drop(driver->game);
	free(driver);
}

int8_t
driver_main(struct Driver *driver) {
	assert(driver);
	struct Cmd *cmd = cmd_new();
	if (!cmd) {
		return EX_OSERR;
	}
	char *str;
	// We don't care at all about the string length, but getline doesn't accept
	// NULL arguments.
	size_t foobar;
	int8_t error_status = EX_OK;
	printf("Welcome to the Z64C chess engine!\n"
		   "You can see a list of available commands "
		   "by typing 'HELP'.\n");
	while (driver->mode != MODE_QUIT) {
		if (getline(&str, &foobar, stdin) == -1) {
			return EX_IOERR;
		} else if (driver_run_str(driver, str) == EX_USAGE) {
			printf("error \"Invalid syntax.\"\n");
		}
	}
	// Unreachable code.
	return EX_SOFTWARE;
}

int8_t
driver_run_str(struct Driver *driver, char *str) {
	assert(driver);
	assert(str);
	struct Cmd *cmd = cmd_new();
	int8_t exit_status = cmd_parse(cmd, str);
	if (exit_status != 0) {
		return exit_status;
	}
	char *cmd_arg_0 = cmd_arg_ith(cmd, 0);
	if (!cmd_arg_0 || cmd_arg_0[0] == '#') {
		return EX_USAGE;
	}
	switch (XXH64(cmd_arg_0, strlen(cmd_arg_0), 0)) {
		case 0x33bf00a859c4ba3f:
			return driver_call_foo(driver, cmd);
		case 0x757dfc0052b6ac66:
			return driver_call_load(driver, cmd);
		case 0xec22b9453ad51a5a:
			return driver_call_move(driver, cmd);
		case 0x021bbb94289971b4:
			return driver_call_print(driver, cmd);
		case 0xd795d67f2e9e6996:
			return driver_call_save(driver, cmd);
		case 0x24115d14507a61eb:
			return driver_call_set(driver, cmd);
		case 0xde2128284d8fdb3c:
			return driver_call_setup(driver, cmd);
		case 0xca46a094b59c646f:
			return driver_call_think(driver, cmd);
#if Z64C_SWITCH_TRAIN
		case 0xd18f9b6611eb8e16:
			return driver_call_train(driver, cmd);
#endif
		case 0x707db5f765aed6d8:
			return driver_call_quit(driver, cmd);
		default:
			fprintf(stderr,
				    "error \"'%s' is not a known UCI command.\"\n", cmd_arg_0);
			return EX_OK;
	}
}
