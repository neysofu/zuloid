/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_EVALFN_H
#define Z64C_EVAL_EVALFN_H

#include "chess/position.h"
#include "eval/eval.h"

struct EvalFn;

struct EvalFn *
evaln_new(void);

void
evalfn_delete(struct EvalFn *evalfn);

int
evalfn_run(struct EvalFn *evalfn, struct Position *position, struct Eval *eval);

#endif
