#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *
read_line(void)
{
	size_t i = 0;
	size_t capacity = 8;
	char *buffer = malloc(capacity);
	char c;
	while (!feof(stdin) && (c = getchar()) != '\n') {
		buffer[i++] = c;
		if (i == capacity) {
			capacity *= 2;
			buffer = realloc(buffer, capacity);
		}
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
exit_if_null(void *ptr) {
	if (!ptr) {
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
