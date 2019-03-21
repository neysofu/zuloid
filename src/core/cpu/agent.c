/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/agent.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "core/cpu/evaluation.h"
#include "core/cpu/tensor.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Agent
{
	FILE *source;
	size_t tensors_count;
	struct Tensor *tensors;
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		*agent = (struct Agent){
			.source = NULL, .tensors_count = 0, .tensors = NULL,
		};
	}
	return agent;
}

void
agent_delete(struct Agent *agent)
{
	if (!agent) {
		return;
	}
	free(agent->tensors);
	free(agent);
}

int
agent_import(struct Agent *agent, FILE *file)
{
	fseek(file, 0, SEEK_END);
	size_t file_length = ftell(file);
	char *buffer = malloc(file_length);
	// FIXME: malloc check
	fread(buffer, 1, file_length, file);
	fclose(file);
	cJSON *json = cJSON_Parse(buffer);
	cJSON *layer_0 = cJSON_GetObjectItem(json, "layer_0");
	agent->tensors_count = cJSON_GetArraySize(layer_0);
	agent->tensors = malloc(sizeof(int64_t) * agent->tensors_count);
	// FIXME: malloc check
	return 0;
}

int
agent_export(struct Agent *agent, FILE *file)
{
	UNUSED(agent);
	fclose(file);
	return 0;
}

enum
{
	BATCH_SIZE = 16,
};

int
agent_eval_position(struct Agent *agent, struct Position *position)
{
	UNUSED(agent);
	UNUSED(position);
	/* N and M dimension tensors, I need to convert one into the other. Great.
	 * Every neuron bit has M XNOR connections. I need to use POPCNT, so I need
	 * to focus on output neurons one at a time.
	 * 1. Count all firing connections to neuron. If it's enough, then keep it.
	 * */
	// struct Tensor tensor = {
	//	128 * 64, malloc(128 * 64),
	//};
	// size_t counts[BATCH_SIZE] = { 0 };
	return 0;
}
