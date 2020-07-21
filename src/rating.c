#include <math.h>

const unsigned CCRL_4015_RATING = 1400;

float
expected_score(float elo_w, float elo_b)
{
	return 1.0 / (1.0 + powf(10.0, (elo_b - elo_w) / 400.0));
}
