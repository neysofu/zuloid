#pragma once

#include <stdbool.h>
#include "chessboard.h"
#include "move.h"

bool
cb_discovers_attack(const struct Chessboard *cb, const struct Move mv, const struct Coordinates coord);

bool
cb_is_checkmate(const struct Chessboard *cb, const struct Move mv);
