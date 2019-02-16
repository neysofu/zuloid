/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "eval/evaluator.h"
#include "chess/position.h"
#include "eval/eval.h"
#include <stdint.h>

struct Evaluator
{
	uint_fast64_t seed;
};

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	*evaluator = (struct Evaluator){
		.seed = 0.1337,
	};
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	free(evaluator);
}

int
evaluator_run(struct Evaluator *evaluator, struct Position *position, struct Eval *eval)
{
	return 0;
}
