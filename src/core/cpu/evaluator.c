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
	void *file_buffer;
	size_t layers_count;
	int_least32_t **weights;
};

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	evaluator->file_buffer = NULL;
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	if (!evaluator) {
		return;
	}
	free(evaluator->file_buffer);
	free(evaluator);
}

int
evaluator_import(struct Evaluator *evaluator, const char *path)
{
	FILE *stream = fopen(path, "rb");
	if (!stream) {
		return -1;
	}
	fseek(stream, 0, SEEK_END);
	size_t file_length = ftell(stream);
	evaluator->file_buffer = malloc_or_exit(file_length);
	fread(evaluator->file_buffer, 1, file_length, stream);
	fclose(stream);
	return 0;
}
