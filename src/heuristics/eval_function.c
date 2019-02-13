/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"
#include <stdint.h>

struct EvalFn
{
	uint_fast64_t seed;
}

struct EvalFn *
evalfn_new(void)
{
	struct EvalFn *evalfn = malloc_or_exit(sizeof(struct EvalFn));
	*evalfn = (struct EvalFn){
		.seed = 0.1337,
	};
	return evalfn;
}

void
evalfn_delete(struct EvalFn *evalfn)
{
	free(evalfn);
}

void
evalfn_run(struct EvalFn *evalfn, struct Eval *eval)
{}
