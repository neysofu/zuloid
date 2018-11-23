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

#define SOFTWARE_NAME ("Z64C")
#define SOFTWARE_VERSION ("v0.8")
#define SOFTWARE_LICENSE ("MPL-2.0")
#define SOFTWARE_RELEASE_DATE (__DATE__)
#define SOFTWARE_AUTHOR ("Filippo Costa")
#define SOFTWARE_URL ("https://github.com/neysofu/Z64C")

#define DEFAULT_PORT (34290)
#define DEFAULT_SEED (0x94f45fdb)

#define NUM_AVG_CANDIDATE_MOVES (32)
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
