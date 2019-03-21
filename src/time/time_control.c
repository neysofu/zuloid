/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "time/time_control.h"
#include "cJSON/cJSON.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

struct TimeControl *
time_control_new_bullet(void)
{
	struct TimeControl *tc = malloc(sizeof(struct TimeControl));
	if (tc) {
		*tc = (struct TimeControl){
			.time_limit_in_seconds = 60,
		};
	}
	return tc;
}

struct TimeControl *
time_control_new_from_json(const cJSON *json)
{
	struct TimeControl *time_control = time_control_new_bullet();
	if (time_control) {
		cJSON *time_limit = cJSON_GetObjectItem(json, "time_limit");
		cJSON *increment = cJSON_GetObjectItem(json, "increment");
		cJSON *delay = cJSON_GetObjectItem(json, "delay");
		if (time_limit) {
			time_control->time_limit_in_seconds = time_limit->valuedouble;
		}
		if (increment) {
			time_control->increment_in_seconds = increment->valuedouble;
		}
		if (delay) {
			time_control->delay_in_seconds = delay->valuedouble;
		}
	}
	return time_control;
}

void
time_control_delete(struct TimeControl *tc)
{
	if (!tc) {
		return;
	}
	while (tc->next_time_control) {
		tc = tc->next_time_control;
	}
	while (tc->previous_time_control) {
		tc = tc->previous_time_control;
		free(tc->next_time_control);
	}
	free(tc);
}
