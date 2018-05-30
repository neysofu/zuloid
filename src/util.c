#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 1024

char *
read_line(char *ptr) {
	if (!ptr) {
		return NULL;
	}
	size_t line_lenght = 0;
	while (true) {
		char c = getchar();
		*(ptr+line_lenght) = c;
		if ((++line_lenght) >= LINE_BUFFER_SIZE) {
			ptr = realloc(ptr, LINE_BUFFER_SIZE);
			if (!ptr) {
				return NULL;
			}
		}
		if (c == EOF || c == '\n') {
			break;
		}
	}
	*(ptr+line_lenght) = '\0';
	return ptr;
}
