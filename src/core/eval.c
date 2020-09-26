#include "core/eval.h"
#include "agent.h"
#include "base64/base64.h"
#include "cJSON/cJSON.h"
#include "chess/bb.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "chess/position.h"
#include "engine.h"
#include "eval.h"
#include "libpopcnt/libpopcnt.h"
#include "mt-64/mt-64.h"
#include "utils.h"
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float WEIGHTS_BY_POS[SQUARES_COUNT] = {
	0.4, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.4, //
	0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, //
	0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, //
	0.7, 0.7, 0.7, 0.8, 0.8, 0.7, 0.7, 0.7, //
	0.7, 0.7, 0.7, 0.8, 0.8, 0.7, 0.7, 0.7, //
	0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, //
	0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, //
	0.4, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.4, //
};

float
eval_bbwscore(Bitboard bb, float weight)
{
	Square square = 0;
	float score = 0.0;
	while (bb) {
		POP_LSB(square, bb);
		score += WEIGHTS_BY_POS[square] * weight;
	}
	return score;
}

float
position_eval_color(const struct Board *pos, enum Color side)
{
	return eval_bbwscore(pos->bb[side] & pos->bb[PIECE_TYPE_PAWN], 1.0) +
	       eval_bbwscore(pos->bb[side] & pos->bb[PIECE_TYPE_KNIGHT], 3.0) +
	       eval_bbwscore(pos->bb[side] & pos->bb[PIECE_TYPE_BISHOP], 3.1) +
	       eval_bbwscore(pos->bb[side] & pos->bb[PIECE_TYPE_KING], 1000.0) +
	       eval_bbwscore(pos->bb[side] & pos->bb[PIECE_TYPE_ROOK], 5.0);
}

float
position_eval_turn(const struct Board *pos)
{
	const float scores[COLORS_COUNT] = { 0.18, -0.18 };
	return scores[pos->side_to_move];
}

float
position_eval(const struct Board *pos)
{
	float w_score = position_eval_color(pos, COLOR_WHITE);
	float b_score = position_eval_color(pos, COLOR_BLACK);
	return w_score - b_score + position_eval_turn(pos);
}
