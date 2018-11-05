/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "switches.h"
#include <stdio.h>

#if SWITCH_LOG
#define LOG_DEBUG(fmt, args...)                                             \
	fprintf(stderr,                                                              \
	        "[DEBUG] %s:%s:%d -- " fmt,                                          \
	        __FILE__ + __FILE_PREFIX_SIZE__,                                     \
	        __func__,                                                            \
	        __LINE__,                                                            \
	        ##args);
#define LOG_INFO(fmt, args...)                                             \
	fprintf(stderr,                                                              \
	        "[INFO] %s:%s:%d -- " fmt,                                          \
	        __FILE__ + __FILE_PREFIX_SIZE__,                                     \
	        __func__,                                                            \
	        __LINE__,                                                            \
	        ##args);
#define LOG_WARN(fmt, args...)                                             \
	fprintf(stderr,                                                              \
	        "[WARN] %s:%s:%d -- " fmt,                                          \
	        __FILE__ + __FILE_PREFIX_SIZE__,                                     \
	        __func__,                                                            \
	        __LINE__,                                                            \
	        ##args);
#define LOG_ERROR(fmt, args...)                                             \
	fprintf(stderr,                                                              \
	        "[ERROR] %s:%s:%d -- " fmt,                                          \
	        __FILE__ + __FILE_PREFIX_SIZE__,                                     \
	        __func__,                                                            \
	        __LINE__,                                                            \
	        ##args);
#define LOG_FATAL(fmt, args...)                                             \
	fprintf(stderr,                                                              \
	        "[FATAL] %s:%s:%d -- " fmt,                                          \
	        __FILE__ + __FILE_PREFIX_SIZE__,                                     \
	        __func__,                                                            \
	        __LINE__,                                                            \
	        ##args);
#else
#define LOG_DEBUG(fmt, args...)
#define LOG_INFO(fmt, args...)
#define LOG_ERROR(fmt, args...)
#define LOG_FATAL(fmt, args...)
#endif
