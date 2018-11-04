/**
 * @file globals.h
 * @brief Global constants.
 */

#pragma once

#include <stdlib.h>

extern const char *const engine_name;
extern const char *const engine_version;
extern const char *const engine_license;
extern const char *const engine_release_date;
extern const char *const engine_author;
extern const char *const engine_url;
extern const char *const engine_language;

#define ENGINE_NAME "Z64C"
#define ENGINE_VERSION "0.8"
#define ENGINE_LICENSE "GNU AGPLv3"
#define ENGINE_RELEASE_DATE __DATE__
#define ENGINE_AUTHOR "Filippo Costa (@neysofu)"
#define ENGINE_URL "https://zsixfourc.net"
#define ENGINE_LANGUAGE "en-us"

extern const uint32_t default_port;
extern const uint32_t default_seed;

#define DEFAULT_PORT 34290
#define DEFAULT_SEED 0x94f45fdb
#define NUM_AVG_CANDIDATE_MOVES 32
#define PAGE_SIZE sysconf(_SC_PAGESIZE)
