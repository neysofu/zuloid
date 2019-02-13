/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chess/position.h"

struct EvalFunction;

struct EvalFunction *
eval_function_new(void);

void
eval_function_delete(struct EvalFunction *eval_function);

struct Eval
eval_function_run(struct EvalFunction *eval_function, struct Position *position);
