#include <math.h>

const unsigned CCRL_4015_RATING = 1400;

float
expected_score(unsigned elo_w, unsigned elo_b)
{
	float elo_delta = (float)elo_b - (float)elo_w;
	return 1.0 / (1.0 + powf(10.0, elo_delta / 400.0));
}
