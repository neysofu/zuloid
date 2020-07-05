#ifndef ZORRO_CHESS_FEN_H
#define ZORRO_CHESS_FEN_H

#include "chess/position.h"

/*  64 + | pieces
 *   7 + | slashes
 *   1 + | space
 *   1 + | active color
 *   1 + | space
 *   4 + | castling rights
 *   1 + | space
 *   2 + | en-passant target square
 *   1 + | space
 *   6 + | half-moves clock
 *   1 + | space
 *   6 + | full-moves
 * -----
 *  95 (max) plus some extra safety space. */

enum
{
	FEN_SIZE = 128
};

/*  */
char *
fen_from_position(char *buf, const struct Board *pos, char sep);

int
position_init_from_fen_fields(struct Board *pos, char **fieldsptr);

int
position_init_from_fen(struct Board *pos, char *fen);

/* Pretty printing to stdout for debug purposes. */
void
position_print(struct Board *pos);

#endif
