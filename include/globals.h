/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_GLOBALS_H
#define Z64C_GLOBALS_H

#include <stdlib.h>

/* Size of pointer types in bits. Currently used for cache implementation
 * details. */
extern const int ADDRESS_SIZE;

extern const char *const Z64C_COPYRIGHT;
extern const char *const Z64C_LICENSE;
/* Compiler build timestamp in YYYY-MM-DD format. */
extern const char *const Z64C_BUILD_DATE;
extern const char *const Z64C_URL;
extern const char *const Z64C_BACKEND_NAME;
/* Software versioning as in MAJOR.MINOR.PATCH. */
extern const char *const Z64C_VERSION;

#endif
