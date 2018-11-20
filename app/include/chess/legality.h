/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file move_inspector.h
 * @brief A simple object to unpack information that arises during move
 * checki
 */

#pragma once

#include <stdbool.h>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/result.h"

bool
board_approves_pseudolegal_move(struct Board *board,
                                Move move,
                                enum Piece piece);

bool
board_approves_legal_move(struct Board *board, Move move, enum Piece piece);

struct Result
board_push_pseudolegal_move(struct Board *board, Move move, enum Piece piece);

struct Result
board_push_legal_move(struct Board *board, Move move, enum Piece piece);
