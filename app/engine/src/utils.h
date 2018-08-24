#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define WHITESPACE_CHARS " \t\n\v\f\r"

// Check if the stdin buffer contains some text.
bool
is_stdin_empty(void);

// Try to fetch the home directory first from environment variables, then from
// UID.
char *
home_dir(void);

// Get the current UNIX-like timestamp in milliseconds.
size_t
timestamp_msec(void);

// Get the page size on this OS (usually 4kiB).
size_t
page_size(void);
