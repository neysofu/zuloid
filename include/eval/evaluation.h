/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_EVALUATION_H
#define Z64C_EVAL_EVALUATION_H

#include "chess/move.h"
#include <stdint.h>

struct Evaluation
{
	int_fast32_t score;
	float dispersion;
	struct Move candidate_moves[256];
};

#endif
