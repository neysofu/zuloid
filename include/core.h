/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#ifndef Z64C_CORE_H
#define Z64C_CORE_H

#include "chess/color.h"
#include "chess/position.h"
#include <stdint.h>
#include <stdlib.h>

float
position_eval(struct Position *pos);
float
position_eval_color(struct Position *pos, enum Color color);

#endif
