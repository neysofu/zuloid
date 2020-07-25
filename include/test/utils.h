#ifndef ZORRO_TEST_UTILS_H
#define ZORRO_TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>

struct Lines;

char *
lines_nth(struct Lines *lines, size_t i);

size_t
lines_count(struct Lines *lines);

void
lines_delete(struct Lines *lines);

struct Lines *
file_line_by_line(FILE *stream);

#endif
