#ifndef ZULOID_CORE_EVAL_H
#define ZULOID_CORE_EVAL_H

#include "chess/color.h"
#include "chess/position.h"

float
position_eval_color(const struct Board *pos, enum Color side);

float
position_eval(const struct Board *pos);

#endif
