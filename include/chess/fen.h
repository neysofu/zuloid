#ifndef ZORRO_CHESS_FEN_H
#define ZORRO_CHESS_FEN_H

#include "chess/position.h"
#include <stdio.h>

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

// Pretty printing to FILE stream for debug purposes.
void
position_pprint(FILE *stream, struct Board *pos);

#endif
