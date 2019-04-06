/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "cJSON/cJSON.h"
#include "utils/utils.h"

struct Tensor
{
	size_t size;
	void *data;
};

int
tensor_read_from_json(struct Tensor *tensor, cJSON *json);

int
tensor_from_position(struct Tensor *tensor, const struct Position *position);

int
tensor_multiply(struct Tensor *tensor,
                struct Tensor *weights,
                struct Tensor *buffer,
                size_t n);
