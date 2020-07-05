#ifndef ZORRO_GLOBALS_H
#define ZORRO_GLOBALS_H

#include <stdlib.h>

/* Size of pointer types in bits. Currently used for cache implementation
 * details. */
extern const int ADDRESS_SIZE;

extern const char *const ZORRO_COPYRIGHT;
extern const char *const ZORRO_LICENSE;
/* Compiler build timestamp in YYYY-MM-DD format. */
extern const char *const ZORRO_BUILD_DATE;
extern const char *const ZORRO_URL;
extern const char *const ZORRO_BACKEND_NAME;
/* Software versioning as in MAJOR.MINOR.PATCH. */
extern const char *const ZORRO_VERSION;

#define MAX_DEPTH 20

#endif
