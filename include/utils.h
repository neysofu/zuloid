#ifndef ZORRO_UTILS_H
#define ZORRO_UTILS_H

#include <stdio.h>
#include <stdint.h>

/* Catch-all error codes for the whole codebase. Add new cases as you please. You should
 * probably restrain from showing it to the end user though. */
enum ErrorCode
{
	ERR_CODE_NONE,
	ERR_CODE_EOF,
	ERR_CODE_ALLOC,
	ERR_CODE_UNSUPPORTED,
	ERR_CODE_INVALID_FEN,
};

char *
read_line(void);

char *
strtok_whitespace(char *str);

uint16_t
djb_hash(const char *str);

#endif
