#ifndef ZULOID_UTILS_H
#define ZULOID_UTILS_H

#include <stdint.h>
#include <stdio.h>

#define UNUSED(x) (void)(x)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Catch-all error codes for the whole codebase. Add new cases as you please.
 * You should probably restrain from showing it to the end user though. */
enum ErrorCode
{
	ERR_CODE_NONE,
	ERR_CODE_EOF,
	ERR_CODE_ALLOC,
	ERR_CODE_UNSUPPORTED,
	ERR_CODE_INVALID_FEN,
};

char *
read_line(FILE *stream);

char *
strtok_whitespace(char *str);

void *
exit_if_null(void *ptr);

uint16_t
djb_hash(const char *str);

int
streq(const char *s1, const char *s2);

#endif
