#include "test/utils.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

struct Lines
{
	char **data;
	size_t length;
	size_t capacity;
};

size_t
lines_count(struct Lines *lines)
{
	return lines->length;
}

char *
lines_nth(struct Lines *lines, long long i)
{
	if (i < 0) {
		i += lines->length;
	}
	if ((size_t)i >= lines->length || i < 0) {
		return NULL;
	}
	return lines->data[i];
}

void
lines_delete(struct Lines *lines)
{
	for (size_t i = 0; i < lines->length; i++) {
		free(lines->data[i]);
	}
	free(lines);
}

void
set_null_lines(struct Lines *lines)
{
	for (size_t i = lines->length; i < lines->capacity; i++) {
		lines->data[i] = NULL;
	}
}

struct Lines *
file_line_by_line(FILE *stream)
{
	long position = ftell(stream);
	rewind(stream);
	struct Lines *lines = exit_if_null(malloc(sizeof(struct Lines)));
	*lines = (struct Lines){
		.data = exit_if_null(malloc(8 * sizeof(char *))),
		.length = 0,
		.capacity = 8,
	};
	set_null_lines(lines);
	char *buffer = NULL;
	while ((buffer = read_line(stream))) {
		lines->data[(lines->length)++] = buffer;
		if (lines->length == lines->capacity) {
			lines->capacity *= 2;
			lines->data =
			  exit_if_null(realloc(lines->data, lines->capacity * sizeof(char *)));
			set_null_lines(lines);
		}
	}
	fseek(stream, position, SEEK_SET);
	return lines;
}
