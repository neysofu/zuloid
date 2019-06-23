/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* strtok is abominable and I hate it with the passion of a thousand burning
 * suns. That said, we are using it in a controlled setting (i.e. a small
 * executable) and I believe it's fine. */
char *
strtok_whitespace(char *str)
{
	return strtok(str, " \t\v\r\n");
}
