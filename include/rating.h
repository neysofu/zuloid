#ifndef ZORRO_RATING_H
#define ZORRO_RATING_H

#include <math.h>

extern const unsigned CCRL_4015_RATING;

float
expected_score(unsigned elo_w, unsigned elo_b);

#endif
