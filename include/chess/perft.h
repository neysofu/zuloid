#ifndef ZULOID_CHESS_PERFT_H
#define ZULOID_CHESS_PERFT_H

#include "chess/color.h"
#include "chess/coordinates.h"
#include "chess/move.h"
#include "chess/position.h"
#include <stdbool.h>
#include <stdio.h>

size_t
position_perft(FILE *stream, struct Board *pos, size_t depth);

#endif
