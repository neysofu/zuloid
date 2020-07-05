#include "utils.h"

char *
read_line(void) {
   size_t i = 0;
   size_t capacity = 8;
   // Note that 'realloc(NULL, ...)' is equivalent to 'malloc(...)'.
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
