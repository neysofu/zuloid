/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file utils.h
 * @brief General utility functions.
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define LIKELY(x) (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))
#define POPCOUNT(x) (__builtin_popcountll(x))

/**
 * WATCH OUT FOR UNDEFINED BEHAVIOUR WHEN x == 0!
 */
#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))

#define WHITESPACE_CHARS (" \t\n\v\f\r")

// Check if the stdin buffer contains some text.
bool
util_is_stdin_empty(void);

// Get the current UNIX-like timestamp in milliseconds.
size_t
util_timestamp_msec(void);

bool
util_str_is_whitespace(const char *str);

/**
 * @brief A safe @c malloc wrapper. Z64C has no special stability needs and I
 * don't feel comfortable cluttering the source code with unnecessary NULL
 * checks that don't actually solve anything.
 */
void *
xmalloc(size_t size);