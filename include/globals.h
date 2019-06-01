/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

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
