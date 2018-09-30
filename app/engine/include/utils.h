/// @file utils.h
/// @brief General utility functions.

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define WHITESPACE_CHARS " \t\n\v\f\r"

// Check if the stdin buffer contains some text.
bool
util_is_stdin_empty(void);

// Try to fetch the home directory first from environment variables, then from
// UID. Return NULL if no home directory is found.
char *
util_home_dir(void);

// Get the current UNIX-like timestamp in milliseconds.
size_t
util_timestamp_msec(void);

/// @brief Gets the page size on this device.
/// @return The page size in bytes.
size_t
util_page_size(void);
