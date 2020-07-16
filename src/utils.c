#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *
read_line(FILE *stream)
{
	size_t i = 0;
	size_t capacity = 8;
	char *buffer = exit_if_null(malloc(capacity));
	int c;
	while (!feof(stream) && (c = getc(stream)) != '\n' && c != EOF) {
		buffer[i++] = c;
		if (i == capacity) {
			capacity *= 2;
			buffer = exit_if_null(realloc(buffer, capacity));
		}
	}
	if (i == 0) {
		free(buffer);
		return NULL;
	}
	buffer[i] = '\0';
	return buffer;
}

uint16_t
djb_hash(const char *str)
{
	uint16_t hash = 5381;
	for (size_t i = 0; str[i]; i++) {
		hash += (hash << 5) + str[i];
	}
	return hash;
}

void *
exit_if_null(void *ptr)
{
	if (!ptr) {
		// OOM: Out Of Memory.
		puts("[ERROR] OOM condition.");
		exit(1);
	}
	return ptr;
}

// struct UciStrIter
//{
//	char *string;
//	char *state;
//};

// char *
// uci_str_iter(struct UciStrIter *iter)
//{
//	strtok_whitespace(NULL);
//	const char *whitespace = " \t\v\r\n";
//	char *end;
//
//	if (!*string) {
//		iter->state = string;
//		return NULL;
//	}
//
//	iter->string += strspn(s, whitespace);
//
//	if (*s == '\0') {
//		iter->state = string;
//		return NULL;
//	}
//
//	end = string + strcspn(s, whitespace);
//	if (*end == '\0') {
//		iter->state = end;
//		return s;
//	}
//	*end = '\0';
//	iter->state = end + 1;
//	return start;
//}

char *
strtok_whitespace(char *str)
{
	return strtok(str, " \t\v\r\n");
}
