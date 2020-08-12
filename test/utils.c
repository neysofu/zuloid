#include "test/utils.h"
#include "engine.h"
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
	struct Lines *lines = malloc(sizeof(struct Lines));
	exit_if_null(lines);
	*lines = (struct Lines){
		.data = malloc(8 * sizeof(char *)),
		.length = 0,
		.capacity = 8,
	};
	exit_if_null(lines->data);
	set_null_lines(lines);
	char *buffer = NULL;
	while ((buffer = read_line(stream)) && (strlen(buffer) > 0)) {
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

struct Engine *
engine_new_tmp(const char *filename)
{
	struct Engine *engine = engine_new();
	engine->output = fopen(filename, "w+");
	if (!engine->output) {
		exit(1);
	}
	return engine;
}
