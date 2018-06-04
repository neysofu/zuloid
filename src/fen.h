#pragma once

#include "board.h"

struct Board *
fen_to_board(char *fen);

char *
board_to_fen(const struct Board *board);

void
fen_specify_pieces(struct Board *board, char *fen_fragment);

void
fen_specify_active_player(struct Board *board, char *fen_fragment);

void
fen_specify_castling_rights(struct Board *board, char *fen_fragment);

void
fen_specify_en_passant_target(struct Board *board, char *fen_fragment);

void
fen_specify_half_moves(struct Board *board, char *fen_fragment);
