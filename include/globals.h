#ifndef ZULOID_GLOBALS_H
#define ZULOID_GLOBALS_H

#include <stdlib.h>

/* Size of pointer types in bits. Currently used for cache implementation
 * details. */
extern const int ADDRESS_SIZE;

extern const char *const ZULOID_COPYRIGHT;
extern const char *const ZULOID_LICENSE;
/* Compiler build timestamp in YYYY-MM-DD format. */
extern const char *const ZULOID_BUILD_DATE;
extern const char *const ZULOID_URL;
extern const char *const ZULOID_BACKEND_NAME;
extern const char *const ZULOID_VERSION;
extern const char *const ZULOID_VERSION_VERBOSE;

#define MAX_DEPTH 20

#endif
