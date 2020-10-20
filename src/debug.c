/* SPDX-License-Identifier: GPL-3.0-only */

#include "feature_flags.h"
#include <stdarg.h>
#include <stdio.h>

void
debug_printf(const char *format, ...)
{
#if ZULOID_ENABLE_DEBUG_MESSAGES
	va_list args;
	va_start(args, format);
	fprintf(stdout, "[DEBUG] ");
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}
