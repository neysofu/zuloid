#include "core/eval.h"
#include "mt-64/mt-64.h"
#include "agent.h"
#include "base64/base64.h"
#include "cJSON/cJSON.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "engine.h"
#include "eval.h"
#include "libpopcnt/libpopcnt.h"
#include "utils.h"
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float
position_eval_color(const struct Board *pos, enum Color side)
{
	return popcnt64(pos->bb[side] & pos->bb[PIECE_TYPE_PAWN]) +
	       popcnt64(pos->bb[side] & pos->bb[PIECE_TYPE_KNIGHT]) * 3 +
	       popcnt64(pos->bb[side] & pos->bb[PIECE_TYPE_BISHOP]) * 3.2 +
	       popcnt64(pos->bb[side] & pos->bb[PIECE_TYPE_KING]) * 1000 +
	       popcnt64(pos->bb[side] & pos->bb[PIECE_TYPE_ROOK]) * 5;
}

float
position_eval(const struct Board *pos)
{
	return position_eval_color(pos, COLOR_WHITE) - position_eval_color(pos, COLOR_BLACK);
}
