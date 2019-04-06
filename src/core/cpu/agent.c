/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "core/agent.h"
#include "cJSON/cJSON.h"
#include "chess/position.h"
#include "core/eval.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Agent
{
	FILE *source;
	uint64_t l0_or[128];
	uint64_t l0_and[128];
	uint64_t layer[128];
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
	cJSON *layer_0 = cJSON_GetObjectItem(json, "layer_0");
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
