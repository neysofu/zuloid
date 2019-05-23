#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *
strtok_whitespace(char *str)
{
	return strtok(str, " \t\v\r\n");
}
