#include <stdbool.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"

void
globals_init(void) {
	HOME_DIR = home_dir();
	PAGE_SIZE = page_size();
}
