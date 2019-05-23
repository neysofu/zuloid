#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *
strtok_whitespace(char *str)
{
	char *token = strtok(str, " \t\v\r\n");
	printf("token is: %s\n", token);
	return token;
}
