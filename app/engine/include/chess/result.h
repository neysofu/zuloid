/// @file result.h

#pragma once

#include "color.h"
#include "termination.h"

// One amongst every possible game result.
struct Result {
	enum Color winner;
	enum Termination termination;
};

// The result of a game that is not over yet.
extern const struct Result RESULT_NONE;
