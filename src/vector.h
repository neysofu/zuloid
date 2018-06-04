#pragma once

#include <stdbool.h>
#include "board.h"
#include "move.h"

// Checks if a certain move poses a direct threat to a square.
bool
cb_discovers_attack(const struct Chessboard *cb, const struct Move mv, const struct Coordinates coord);

bool
cb_is_checkmate(const struct Chessboard *cb, const struct Move mv);
