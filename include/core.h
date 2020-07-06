#ifndef ZORRO_CORE_H
#define ZORRO_CORE_H

#include "chess/color.h"
#include "chess/position.h"
#include <stdint.h>
#include <stdlib.h>

float
position_eval(struct Board *pos);
float
position_eval_color(struct Board *pos, enum Color color);

#endif
