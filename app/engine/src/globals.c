#include <stdbool.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"

void
globals_init(void) {
	static bool flag = false;
	if (flag) {
		return;
	} else {
		flag = true;
	}
	bb_init();
	PAGE_SIZE = util_page_size();
}

size_t PAGE_SIZE;
