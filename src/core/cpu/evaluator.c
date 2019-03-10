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
#include <stdlib.h>

struct Evaluator
{
	FILE *source;
	size_t layer_0_weights_count;
	int_least64_t *layer_0_weights;
};

struct Evaluator *
evaluator_new(void)
{
	struct Evaluator *evaluator = malloc_or_exit(sizeof(struct Evaluator));
	evaluator->layer_0_weights_count = 0;
	evaluator->layer_0_weights = NULL;
	return evaluator;
}

void
evaluator_delete(struct Evaluator *evaluator)
{
	if (!evaluator) {
		return;
	}
	free(evaluator->layer_0_weights);
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
	cJSON *layer_0 = cJSON_GetObjectItem(json, "layer_0");
	evaluator->layer_0_weights_count = cJSON_GetArraySize(layer_0);
	evaluator->layer_0_weights =
	  malloc_or_exit(sizeof(int_least64_t) * evaluator->layer_0_weights_count);
	size_t i = 0;
	cJSON *weight;
	cJSON_ArrayForEach(weight, layer_0)
	{
		evaluator->layer_0_weights[i] = strtoull(weight->valuestring, NULL, 10);
		i++;
	}
	return 0;
}

int
evaluator_export(struct Evaluator *evaluator, const char *path)
{
	FILE *stream = fopen(path, "wb");
	if (!stream) {
		return -1;
	}
	fclose(stream);
	return 0;
}
