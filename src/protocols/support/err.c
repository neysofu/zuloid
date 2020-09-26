/* SPDX-License-Identifier: GPL-3.0-only */

#include "protocols/support/err.h"
#include <stdlib.h>

void
display_err_syntax(FILE *stream)
{
	fputs("[ERROR] Invalid syntax.\n", stream);
}

void
display_err_unspecified(FILE *stream)
{
	fputs("[ERROR] Unspecified error.\n", stream);
}

void
display_err_invalid_command(FILE *stream)
{
	fputs("[ERROR] Invalid command.\n", stream);
}
