/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * General utility functions. */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define LIKELY(x) (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))
#define POPCOUNT(x) (__builtin_popcountll(x))
/* WATCH OUT FOR UNDEFINED BEHAVIOUR WHEN `x == 0`! */
#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))

// Check if the stdin buffer contains some text.
bool
util_is_stdin_empty(void);

/* Gets the current UNIX time in milliseconds. */
size_t
util_timestamp_msec(void);

/* Returns true if it has only whitespace. */
bool
util_str_is_whitespace(const char *str);

/* A thin wrapper over `malloc` that exits on OOM errors and prints a
 * JSON-RPC error. */
void *
xmalloc(size_t size);
