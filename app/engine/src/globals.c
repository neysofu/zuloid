#include <stdbool.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"

size_t PAGE_SIZE;

void
globals_init(void) {
	static bool init = false;
	if (init) {
		return;
	} else {
		init = true;
	}
	bb_init();
	PAGE_SIZE = util_page_size();
}
