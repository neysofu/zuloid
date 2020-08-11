#ifndef ZULOID_CHESS_FEN_H
#define ZULOID_CHESS_FEN_H

#include "chess/position.h"
#include <stdio.h>

enum
{
	FEN_SIZE = 110
};

/*  */
char *
fen_from_position(char *buf, const struct Board *pos, char sep);

int
position_init_from_fen_fields(struct Board *pos, char **fieldsptr);

int
position_init_from_fen(struct Board *pos, char *fen);

#endif
