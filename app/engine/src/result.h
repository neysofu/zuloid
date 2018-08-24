#pragma once

#include "color.h"
#include "termination.h"

// One amongst the possible game results.
struct Result {
	enum Color winner;
	enum Termination termination;
};

// The result of a game that is not over yet.
extern const struct Result RESULT_NONE;
