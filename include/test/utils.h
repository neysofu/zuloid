#ifndef ZORRO_TEST_UTILS_H
#define ZORRO_TEST_UTILS_H

#include "engine.h"
#include <stdlib.h>

struct Lines;

char *
lines_nth(struct Lines *lines, long long i);

void
lines_delete(struct Lines *lines);

struct Lines *
read_last_lines(FILE *stream, size_t n);

#endif
