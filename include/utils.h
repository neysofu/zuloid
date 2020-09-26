#ifndef ZULOID_UTILS_H
#define ZULOID_UTILS_H

#include <ctype.h>
#include <limits.h>
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

static const char *const WHITESPACE = " \t\v\r\n";

char *
strtrim(char *str, const char *trimmable);

char *
strtok_r_whitespace(char *restrict str, char **restrict save);

/*

Case-insensitive string compare (strncmp case-insensitive)
- Identical to strncmp except case-insensitive. See:
http://www.cplusplus.com/reference/cstring/strncmp/
- Aided/inspired, in part, by: https://stackoverflow.com/a/5820991/4561887

str1    C string 1 to be compared
str2    C string 2 to be compared
num     max number of chars to compare

return:
(essentially identical to strncmp)
INT_MIN  invalid arguments (one or both of the input strings is a NULL pointer)
<0       the first character that does not match has a lower value in str1 than in str2
 0       the contents of both strings are equal
>0       the first character that does not match has a greater value in str1 than in str2

*/
static inline int
strncmpci(const char *str1, const char *str2)
{
	int ret_code = INT_MIN;

	// Check for NULL pointers
	if (!str1 || !str2) {
		goto done;
	}

	// Continue doing case-insensitive comparisons, one-character-at-a-time, of str1 to
	// str2, as long as at least one of the strings still has more characters in it, and we
	// have not yet compared num chars.
	while (*str1 || *str2) {
		ret_code = tolower((int)(*str1)) - tolower((int)(*str2));
		if (ret_code != 0) {
			// The 2 chars just compared don't match
			break;
		}
		str1++;
		str2++;
	}

done:
	return ret_code;
}

struct Strings
{
	const char **buffers;
	const int count;
	const int i;
};

struct Strings *
strings_new(void);

void
strings_delete(struct Strings *strings);

const char *
strings_ith(const struct Strings *strings);

const char *
strings_next(struct Strings *strings);

#endif
