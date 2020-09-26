/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_PROTOCOLS_SUPPORT_ERR_H
#define ZULOID_PROTOCOLS_SUPPORT_ERR_H

#include <stdio.h>
#include <stdlib.h>

void
display_err_syntax(FILE *stream);
void
display_err_unspecified(FILE *stream);
void
display_err_invalid_command(FILE *stream);

#endif
