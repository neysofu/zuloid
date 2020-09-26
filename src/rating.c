/* SPDX-License-Identifier: GPL-3.0-only */

#include <math.h>

// Estimated score in the CCRL 40/15 listing: <https://ccrl.chessdom.com/ccrl/4040/>.
const unsigned CCRL_4015_RATING = 1400;

float
expected_score(float elo_white, float elo_black)
{
	// See <https://en.wikipedia.org/wiki/Elo_rating_system#Mathematical_details>.
	return 1.0 / (1.0 + powf(10.0, (elo_black - elo_white) / 400.0));
}
