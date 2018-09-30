#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>
#include "driver.h"
#include "globals.h"

int
main(void) {
	setlinebuf(stdin);
	setlinebuf(stdout);
	setlinebuf(stderr);
	globals_init();
	struct Driver *driver = driver_new();
	if (!driver) {
		return EX_OSERR;
	}
	int8_t exit_status = driver_main(driver);
	driver_drop(driver);
	return exit_status;
}
