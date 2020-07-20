#include "switches.h"
#include <stdio.h>
#include <stdarg.h>

void
debug_printf(const char *format, ...) {
#if DEBUG_MESSAGES
    va_list args;
    va_start(args, format);
    fprintf(stdout, "[DEBUG] ");
    vfprintf(stdout, format, args);
    va_end(args);
#endif
}
