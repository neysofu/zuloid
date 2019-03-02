/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_EVAL_EVALUATOR_H
#define Z64C_EVAL_EVALUATOR_H

#include "chess/position.h"
#include "eval/evaluation.h"
#include <stdio.h>

struct Evaluator;

struct Evaluator *
evaluator_new(void);

void
evaluator_delete(struct Evaluator *evaluator);

int
evaluator_import(struct Evaluator *evaluator, FILE *file);

int
evaluator_export(struct Evaluator *evaluator, FILE *file);

int
evaluator_load_positions(struct Evaluator *evaluator,
                         struct Position *positions,
                         size_t positions_count,
                         struct Evaluation *ptr);

int
evaluator_run(struct Evaluator *evaluator);

#endif
