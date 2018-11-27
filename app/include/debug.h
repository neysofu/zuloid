/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include "switches.h"
#include <stdio.h>

#define DEBUG(fmt, args...)                                                                \
	do {                                                                                   \
		if (SWITCH_DEBUG) {                                                                \
			printf("# %s:%s:%d -- " fmt "\r\n",                                            \
			       __FILE__ + __FILE_PREFIX_SIZE__,                                        \
			       __func__,                                                               \
			       __LINE__,                                                               \
			       ##args);                                                                \
		}                                                                                  \
	} while (0)
