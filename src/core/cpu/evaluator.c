/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/evaluator.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "core/cpu/evaluation.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

struct Evaluator
{
	FILE *import;
	size_t tensors_count;
	struct Tensor **tensors;
};

struct Tensor
{
	size_t weights_count;
	int_least32_t *weights;
};

struct Tensor *
tensor_new_from_json(cJSON *value)
{
	cJSON *weights = cJSON_GetObjectItem(value, "weights");
	struct Tensor *tensor = malloc_or_exit(sizeof(struct Tensor));
	tensor->weights_count = cJSON_GetArraySize(weights);
	tensor->weights = malloc_or_exit(sizeof(int_least32_t) * tensor->weights_count);
	weights = weights->child;
	for (size_t i = 0; i < tensor->weights_count; i++) {
		tensor->weights[i] = weights->valuedouble;
		weights = weights->next;
	}
	return tensor;
}

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	evaluator->tensors_count = 0;
	evaluator->tensors = NULL;
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	if (!evaluator) {
		return;
	}
	free(evaluator->tensors);
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
	char *buffer = malloc_or_exit(file_length);
	fread(buffer, 1, file_length, stream);
	fclose(stream);
	cJSON *json = cJSON_Parse(buffer);
	cJSON *tensor_0 = cJSON_GetObjectItem(json, "tensor_0");
	evaluator->tensors_count = 1;
	evaluator->tensors = malloc_or_exit(sizeof(struct Tensor *) * evaluator->tensors_count);
	evaluator->tensors[0] = tensor_new_from_json(tensor_0);
	return 0;
}
