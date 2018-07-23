#pragma once

#include "score.h"
#include "termination.h"

struct Result {
	enum Score score;
	enum Termination termination;
};

extern const struct Result RESULT_NONE;
