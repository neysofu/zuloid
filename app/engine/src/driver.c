/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "driver.h"
#include "chess/board.h"
#include "chess/fen.h"
#include "chess/move.h"
#include "chess/result.h"
#include "clock.h"
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

struct Driver *
driver_new(void)
{
	struct Driver *driver = xmalloc(sizeof(struct Driver));
	driver->ttable = ttable_new();
	driver->scontroller = scontroller_new(driver->ttable);
	driver->board = BOARD_STARTPOS;
	driver->result = RESULT_NONE;
	driver->mode = MODE_IDLE;
	driver->exit_status = EX_OK;
	settings_default(&driver->settings);
	return driver;
}

void
driver_free(struct Driver *driver)
{
	LOG_DEBUG("Freeing all memory.\n");
	assert(driver);
	ttable_free(driver->ttable);
	scontroller_free(driver->scontroller);
	free(driver);
}

int8_t
driver_main(struct Driver *driver)
{
	LOG_DEBUG("Now entering the main UCI loop.\n");
	assert(driver);
	char *cmd = NULL;
	size_t cmd_length = 0;
	while (driver->mode != MODE_EXIT) {
		if (getline(&cmd, &cmd_length, stdin) == -1) {
			LOG_FATAL("Something bad happened while reading from stdin. Abort.\n");
			return EX_IOERR;
		}
		const char *response = driver_rpc(driver, cmd);
		if (response) {
			LOG_DEBUG("Writing the JSON RPC response object to stdout.\n");
			printf("  %s\r\n", response);
			free((void *)(response));
		}
	}
	return driver->exit_status;
}
