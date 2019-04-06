/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils/dyn_str.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

const char *const WHITESPACE_CHARS = " \t\f\v\r\n";
const size_t DYN_STR_DEFAULT_CAPACITY = 64;

const struct DynStr DYN_STR_EMPTY = {
	.buffer = NULL,
	.length = 0,
	.capacity = 0,
	.i = 0,
};

void
dyn_str_init(struct DynStr *dyn_str, char *string)
{
	assert(dyn_str);
	dyn_str->buffer = string;
	dyn_str->length = strlen(string);
	dyn_str->capacity = dyn_str->length;
	dyn_str->i = 0;
}

void
dyn_str_tokenize(struct DynStr *dyn_str)
{
	assert(dyn_str);
	dyn_str->i = strspn(dyn_str->buffer, WHITESPACE_CHARS);
	for (char *s = dyn_str_current_token(dyn_str); *s; s++) {
		if (isspace(*s)) {
			*s = '\0';
		}
	}
}

char *
dyn_str_next_token(struct DynStr *dyn_str)
{
	assert(dyn_str);
	dyn_str->i += strlen(dyn_str_current_token(dyn_str));
	while (dyn_str->i < dyn_str->capacity && dyn_str->buffer[dyn_str->i] == '\0') {
		dyn_str->i++;
	}
	return dyn_str_current_token(dyn_str);
}

char *
dyn_str_current_token(struct DynStr *dyn_str)
{
	assert(dyn_str);
	return dyn_str->buffer + dyn_str->i;
}

int
dyn_str_resize(struct DynStr *dyn_str, size_t capacity)
{
	assert(dyn_str);
	assert(capacity);
	char *new_buffer = realloc(dyn_str->buffer, capacity);
	if (new_buffer) {
		dyn_str->buffer = new_buffer;
		dyn_str->capacity = capacity;
		return ERR_CODE_NONE;
	} else {
		return ERR_CODE_ALLOC;
	}
}

int
dyn_str_read_line_from_stream(struct DynStr *dyn_str, FILE *stream)
{
	assert(stream);
	assert(dyn_str);
	if (dyn_str->capacity > DYN_STR_DEFAULT_CAPACITY || dyn_str->capacity == 0) {
		if (dyn_str_resize(dyn_str, DYN_STR_DEFAULT_CAPACITY)) {
			return ERR_CODE_ALLOC;
		}
	}
	dyn_str->length = 0;
	char c;
	do {
		c = fgetc(stream);
		if (c == EOF) {
			return ERR_CODE_EOF;
		}
		dyn_str->buffer[dyn_str->length++] = c;
		if (dyn_str->length == dyn_str->capacity) {
			if (dyn_str_resize(dyn_str, dyn_str->capacity * 2)) {
				return ERR_CODE_ALLOC;
			}
		}
	} while (c != '\n');
	return ERR_CODE_NONE;
}
