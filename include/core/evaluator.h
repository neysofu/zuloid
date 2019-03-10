/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_CORE_EVALUATOR_H
#define Z64C_CORE_EVALUATOR_H

#include <stdio.h>

struct Evaluator;

struct Evaluator *
evaluator_new(void);

void
evaluator_delete(struct Evaluator *evaluator);

int
evaluator_import(struct Evaluator *evaluator, const char *path);

int
evaluator_export(struct Evaluator *evaluator, const char *path);

#endif
