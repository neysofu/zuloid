/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "switches.h"
#include <stdio.h>

#if SWITCH_DEBUG
#define debug_printf(fmt, args...)                                             \
	fprintf(                                                                     \
	  stderr, "[DEBUG] %s:%s:%d -- " fmt, __FILE__, __func__, __LINE__, ##args);
#else
#define debug_printf(fmt, args...)
#endif
