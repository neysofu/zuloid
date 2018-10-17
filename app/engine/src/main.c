#include "driver.h"
#include "globals.h"
#include <stdio.h>
#include <sys/select.h>
#include <sysexits.h>

int
main(void)
{
	setlinebuf(stdin);
	setlinebuf(stdout);
	setlinebuf(stderr);
	globals_init();
	struct Driver* driver = driver_new();
	int8_t exit_status = driver_main(driver);
	driver_free(driver);
	return exit_status;
}
