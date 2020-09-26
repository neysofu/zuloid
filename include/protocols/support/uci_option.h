/* SPDX-License-Identifier: GPL-3.0-only */

#ifndef ZULOID_PROTOCOLS_SUPPORT_UCI_OPTION_H
#define ZULOID_PROTOCOLS_SUPPORT_UCI_OPTION_H

#include <stdbool.h>
#include <stdio.h>

enum UciOptionType
{
	UCI_OPTION_TYPE_CHECK,
	UCI_OPTION_TYPE_SPIN,
	UCI_OPTION_TYPE_COMBO,
	UCI_OPTION_TYPE_BUTTON,
	UCI_OPTION_TYPE_STRING,
};

union UciOptionData
{
	struct
	{
		bool default_val;
		int (*setter)(struct Engine *engine, bool value);
	} check;
	struct
	{
		long default_val;
		long min;
		long max;
		int (*setter)(struct Engine *engine, long value);
	} spin;
	struct
	{
		const char *default_val;
		const char *combo_variants;
		int (*setter)(struct Engine *engine, const char *value);
	} string;
	struct
	{
		int (*setter)(struct Engine *engine);
	} button;
};

struct UciOption
{
	const char *name;
	enum UciOptionType type;
	union UciOptionData data;
};

void
ucioption_fprint(const struct UciOption *option, FILE *stream);

bool
ucioption_combo_allows(const struct UciOption *option, const char *variant);

bool
ucioption_spin_allows(const struct UciOption *option, long val);

void
ucioptiondata_fill(union UciOptionData *data, const char *str, enum UciOptionType type);

struct UciOption *
ucioption_find(const struct UciOption[], size_t count, const char *name);

#endif
