#pragma once

#include <stdlib.h>

extern size_t PAGE_SIZE;

// Initializes all global variables. Their values will be constant after.
void
globals_init(void);
