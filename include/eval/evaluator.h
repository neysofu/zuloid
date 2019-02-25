/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_EVALUATOR_H
#define Z64C_EVAL_EVALUATOR_H

#include "chess/position.h"
#include "eval/evaluation.h"
#include <stdio.h>

enum BackEnd
{
	BACKEND_CPU,
	BACKEND_OPENCL,
};

struct Evaluator;

struct Evaluator *
evaluator_new(void);

int
evaluator_save(struct Evaluator *evaluator, FILE *file);

int
evaluator_read(struct Evaluator *evaluator, FILE *file);

void
evaluator_delete(struct Evaluator *evaluator);

int
evaluator_run(struct Evaluator *evaluator,
              struct Position *position,
              struct Evaluation *ptr);

#endif
