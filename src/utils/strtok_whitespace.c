/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *
strtok_whitespace(char *str)
{
	return strtok(str, " \t\v\r\n");
}
