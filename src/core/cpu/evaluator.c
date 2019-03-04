/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/evaluator.h"
#include "chess/position.h"
#include "core/cpu/evaluation.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

struct Evaluator
{
	int_least32_t *tensor;
	char *error;
};

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	free(evaluator);
}

int
evaluator_import(struct Evaluator *evaluator, FILE *file)
{
}
