/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CHESS_GENERATION_H
#define Z64C_CHESS_GENERATION_H

#include "chess/color.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/position.h"
#include <stdbool.h>

bool
board_approves_pseudolegal_move(struct Board *board, Move move, enum Piece piece);

bool
board_approves_legal_move(struct Board *board, Move move, enum Piece piece);

enum Color
board_push_pseudolegal_move(struct Board *board, Move move, enum Piece piece);

enum Color
board_push_legal_move(struct Board *board, Move move, enum Piece piece);

#endif
