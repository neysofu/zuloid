/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base64.c/base64.h"
#include "chess/position.h"
#include "cJSON/cJSON.h"
#include "utils.h"
#include <assert.h>
#include <string.h>

struct Tensor
{
	size_t size;
	void *data;
};

int
tensor_read_from_json(struct Tensor *tensor, cJSON *json)
{
	assert(tensor);
	cJSON *data = cJSON_GetObjectItem(json, "data");
	if (!cJSON_IsString(data)) {
		return -1;
	}
	tensor->size = b64d_size(strlen(data->valuestring));
	tensor->data = malloc_or_exit(tensor->size);
	b64_decode(data->valuestring, strlen(data->valuestring), tensor->data);
	return 0;
}

void
tensor_from_position(struct Tensor *tensor, struct Position *position)
{
	assert(tensor);
	assert(position);
}

int
tensor_multiply(struct Tensor *tensor, struct Tensor weights[], struct Tensor *buffer)
{
	assert(tensor);
	return 0;
}
