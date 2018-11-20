/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file result.h
 */

#pragma once

#include "color.h"
#include "termination.h"

// One amongst every possible game result.
struct Result
{
	enum Color winner;
	enum Termination termination;
};

// The result of a game that is not over yet.
extern const struct Result RESULT_NONE;
