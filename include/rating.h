#ifndef ZULOID_RATING_H
#define ZULOID_RATING_H

// Estimated ELO score based on <https://ccrl.chessdom.com/ccrl/4040/>.
extern const unsigned CCRL_4015_RATING;

float
expected_score(float elo_white, float elo_black);

#endif
