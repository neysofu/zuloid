/* SPDX-License-Identifier: GPL-3.0-only */

#include "time/time_control.h"
#include "cJSON/cJSON.h"
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
