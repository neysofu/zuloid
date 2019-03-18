/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base64.c/base64.h"
#include "cJSON/cJSON.h"
#include "utils.h"
#include <assert.h>

struct Tensor
{
	size_t size;
	void *data;
};

int
tensor_read_from_json(struct Tensor *tensor, cJSON *json)
{
	cJSON *data = cJSON_GetObjectItem(json, "data");
	assert(cJSON_IsString(data));
	tensor->size = b64d_size(strlen(data->valuestring));
	tensor->data = malloc(tensor->size);
	b64_decode(data->valuestring, strlen(data->valuestring), tensor->data);
	return 0;
}

int
tensors_popcnt(struct Tensor *tensor, struct Tensor *weights, size_t *popcnts, size_t n)
{
	assert(tensor);
	//for (int j = 0; j < n; i++) {
	//	for (int i = 0; i < tensor->size; i++) {
	//		popcnts[i] += tensor ^ weights[j][i];
	//	}
	//}
	return 0;
}

tensor_from_popcnt(struct Tensor *tensor, size_t *popcnts)
{
	assert(tensor);
	assert(popcnts);
}
