/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_META_H
#define ZULOID_META_H

#include <stdlib.h>

/* Compiler build timestamp in YYYY-MM-DD format. */
extern const char *const ZULOID_BUILD_DATE;
extern const char *const ZULOID_VERSION;
extern const char *const ZULOID_VERSION_VERBOSE;
extern const unsigned long long ZULOID_PRNG_SEED;

#define MAX_DEPTH 20

#endif
