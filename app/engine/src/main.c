/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bitboards.h"
#include "driver.h"
#include "log.h"
#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>

int
main(void)
{
	log_debug("Now setting line buffering for all three standard channels.\n");
	setlinebuf(stdin);
	setlinebuf(stdout);
	setlinebuf(stderr);
	bb_init();
	struct Driver *driver = driver_new();
	int8_t exit_status = driver_main(driver);
	driver_free(driver);
	return exit_status;
}
