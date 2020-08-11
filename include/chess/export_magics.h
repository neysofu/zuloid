#ifndef ZULOID_CHESS_EXPORT_MAGICS_H
#define ZULOID_CHESS_EXPORT_MAGICS_H

#include "chess/find_magics.h"
#include <stdio.h>

int
magics_export(const struct Magic *magics, const char *identifier, FILE *stream);

#endif
