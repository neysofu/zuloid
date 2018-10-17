#include "globals.h"
#include "bitboards.h"
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>

void
globals_init(void)
{
	debug_printf("Initializing global variables.\n");
	bb_init();
	PAGE_SIZE = util_page_size();
}

size_t PAGE_SIZE;
