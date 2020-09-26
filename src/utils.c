/* SPDX-License-Identifier: GPL-3.0-only */

#include "utils.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
read_line(FILE *stream)
{
	size_t i = 0;
	size_t capacity = 8;
	char *buffer = malloc(capacity);
	exit_if_null(buffer);
	int c;
	while (!feof(stream) && ((c = getc(stream)) != '\n') && c != EOF) {
		buffer[i++] = c;
		if (i == capacity) {
			capacity *= 2;
			buffer = realloc(buffer, capacity);
			exit_if_null(buffer);
		}
	}
	buffer[i] = '\0';
	return buffer;
}

void *
exit_if_null(void *ptr)
{
	if (!ptr) {
		puts("[ERROR] Out-of-memory condition.");
		exit(1);
	}
	return ptr;
}

char *
strtok_whitespace(char *str)
{
	return strtok(str, " \t\v\r\n");
}

char *
strtok_r_whitespace(char *str, char **saveptr)
{
	return strtok_r(str, " \t\v\r\n", saveptr);
}

// Copyright (c) 2005-2014 Rich Felker, et al.
// musl library
char *
strtok_r(char *restrict s, const char *restrict sep, char **restrict p)
{
	if (!s && !(s = *p))
		return NULL;
	s += strspn(s, sep);
	if (!*s)
		return *p = 0;
	*p = s + strcspn(s, sep);
	if (**p)
		*(*p)++ = 0;
	else
		*p = 0;
	return s;
}

char *
strtrim(char *str, const char *trimmable)
{
	// Skip any initial characters in the `trimmable` set.
	str += strspn(str, trimmable);
	// Start from the end and set every byte to the null terminator until a
	// "good" (i.e. non-`trimmmable`) character is found.
	for (char *ptr = str + strlen(str); strchr(trimmable, *ptr); ptr--) {
		*ptr = '\0';
	}
	return str;
}
