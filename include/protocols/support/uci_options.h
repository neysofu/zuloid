#ifndef ZULOID_PROTOCOLS_SUPPORT_UCI_OPTIONS_H
#define ZULOID_PROTOCOLS_SUPPORT_UCI_OPTIONS_H

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
		const char *variants;
		int (*setter)(struct Engine *engine, const char *value);
	} combo;
	struct
	{
		const char *default_val;
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

// Option support is quite hairy and messy. I don't want to break pre-existing
// scripts and configs originally written for other engines.
//
// Please see:
//  - https://komodochess.com/Komodo-11-README.html
//  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
static const struct UciOption UCI_OPTIONS[] = {
	{ .name = "Analysis Contempt",
	  .type = UCI_OPTION_TYPE_COMBO,
	  .data.combo = { .default_val = "Both", .variants = "[Off][White][Black][Both]" } },
	{ .name = "Clear Hash",
	  .type = UCI_OPTION_TYPE_BUTTON,
	  .data.button = { .setter = NULL } },
	{ .name = "Contempt",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 24, .min = -100, .max = 100 } },
	{ .name = "Debug Log File",
	  .type = UCI_OPTION_TYPE_STRING,
	  .data.string = { .default_val = "/tmp/zuloid-tmp" } },
	{ .name = "Hash",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 64, .min = 0, .max = 131072 } },
	{ .name = "Minimum Thinking Time",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 20, .min = 0, .max = 5000 } },
	{ .name = "Move Overhead",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 30, .min = 30, .max = 60000 } },
	{ .name = "nodestime",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 0, .min = 0, .max = 10000 } },
	{ .name = "Ponder",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false } },
	{ .name = "Skill Level",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 20, .min = 0, .max = 20 } },
	{ .name = "Slow Mover",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 84, .min = 10, .max = 1000 } },
	{ .name = "SyzygyPath",
	  .type = UCI_OPTION_TYPE_STRING,
	  .data.string = { .default_val = "<empty>" } },
	{ .name = "SyzygyProbeDepth",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1, .min = 1, .max = 100 } },
	{ .name = "Syzygy50MoveRule",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = true } },
	{ .name = "SyzygyProbeLimit",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 7, .min = 0, .max = 7 } },
	{ .name = "Threads",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1, .min = 1, .max = 512 } },
	{ .name = "UCI_Chess960",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false } },
	{ .name = "UCI_Elo",
	  .type = UCI_OPTION_TYPE_SPIN,
	  .data.spin = { .default_val = 1350, .min = 1350, .max = 2850 } },
	{ .name = "UCI_LimitStrength",
	  .type = UCI_OPTION_TYPE_CHECK,
	  .data.check = { .default_val = false } },
};

#endif
