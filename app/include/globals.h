/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file globals.h
 * @brief Global constants.
 */

#pragma once

#include <stdlib.h>

extern const char *const engine_name;
extern const char *const engine_version;
extern const char *const engine_license;
extern const char *const engine_release_date;
extern const char *const engine_author;
extern const char *const engine_url;

#define DEFAULT_PORT 34290;
#define DEFAULT_SEED 0x94f45fdb;

#define NUM_AVG_CANDIDATE_MOVES 32
#define PAGE_SIZE sysconf(_SC_PAGESIZE)
