#ifndef ZULOID_CHESS_FIND_MAGICS_H
#define ZULOID_CHESS_FIND_MAGICS_H

#include "chess/coordinates.h"
#include "chess/magic.h"
#include <stdint.h>
#include <stdlib.h>

const struct Magic MAGICS[SQUARES_COUNT];
const struct Magic MAGICS_BISHOP[SQUARES_COUNT];

void
magic_find_rook(struct Magic *magic, Square square);

void
magic_find_bishop(struct Magic *magic, Square square);

#endif
