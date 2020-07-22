#include <math.h>

const unsigned CCRL_4015_RATING = 1400;

float
expected_score(float elo_white, float elo_black)
{
	return 1.0 / (1.0 + powf(10.0, (elo_black - elo_white) / 400.0));
}
