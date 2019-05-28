/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/agent.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "core/eval.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//struct Layer
//{
//	size_t con_weights_count;
//	size_t dis_weights_count;
//	int64_t *con_weights;
//	int64_t *dis_weights;
//};
//
//enum ErrorCode
//layer_init_from_json(struct Layer *layer, cJSON *json)
//{
//	assert(layer);
//	cJSON *cwc_item = cJSON_GetObjectItem(json, "con_weights_count");
//	cJSON *dwc_item = cJSON_GetObjectItem(json, "dis_weights_count");
//	assert(cJSON_IsNumber(cwc_item));
//	assert(cJSON_IsNumber(dwc_item));
//	layer->con_weights_count = cwc_item->valuedouble;
//	layer->dis_weights_count = dwc_item->valuedouble;
//	assert(layer->con_weights_count % 64 == 0);
//	assert(layer->dis_weights_count % 64 == 0);
//	layer->con_weights = malloc(layer->con_weights_count / 64);
//	layer->dis_weights = malloc(layer->dis_weights_count / 64);
//	if (!layer->con_weights || !layer->dis_weights) {
//		return ERR_CODE_ALLOC;
//	}
//	cJSON_ArrayForEach(element)
//}
//
//struct Network
//{
//	struct Layer lin;
//	struct Layer l0;
//	struct Layer l1;
//	struct Layer l2;
//	struct Layer l3;
//	struct Layer l4;
//	struct Layer lout;
//};
//
//int
//network_new_from_json(cJSON *json)
//{
//	cJSON *l0 = cJSON_GetObjectItem(json, "l0");
//}

struct Agent
{
	FILE *source;
};

struct Agent *
agent_new(void)
{
	struct Agent *agent = malloc(sizeof(struct Agent));
	if (agent) {
		*agent = (struct Agent){
			.source = NULL,
		};
	}
	return agent;
}

void
agent_delete(struct Agent *agent)
{
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
	// FIXME: malloc check
	return 0;
}

int
agent_export(struct Agent *agent, FILE *file)
{
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
	/* N and M dimension tensors, I need to convert one into the other. Great.
	 * I want a SUPER simple interface... I could use bitshifts for changing
	 * places. So I would need N bitshifts to also influence other bits. How
	 * the hell do I do that? Basically, if there are more than N signals, it's
	 * ok. N is chosen to keep the number of activated neurons stable through
	 * the layers.
	 * to focus on output neurons one at a time.
	 * 1. Count all firing connections to neuron. If it's enough, then keep it. */
	return 0;
}
