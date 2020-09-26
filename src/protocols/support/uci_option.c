/* SPDX-License-Identifier: GPL-3.0-only */

#include "protocols/support/uci_option.h"
#include "engine.h"
#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
ucioption_fprint(const struct UciOption *option, FILE *stream)
{
	fprintf(stream, "option name %s type ", option->name);
	switch (option->type) {
		case UCI_OPTION_TYPE_BUTTON:
			fputs("button\n", stream);
			break;
		case UCI_OPTION_TYPE_SPIN:
			fprintf(stream,
			        "spin default %ld min %ld max %ld\n",
			        option->data.spin.default_val,
			        option->data.spin.min,
			        option->data.spin.max);
			break;
		case UCI_OPTION_TYPE_CHECK:
			fprintf(stream,
			        "check default %s\n",
			        option->data.check.default_val ? "true" : "false");
			break;
		case UCI_OPTION_TYPE_COMBO:
			fprintf(stream, "combo default %s", option->data.string.default_val);
			const char *start = option->data.string.combo_variants;
			while (*start++ == '[') {
				const int length = strcspn(start, "]");
				fprintf(stream, " var %.*s", length, start);
				start += length + 1;
			}
			fputc('\n', stream);
			break;
		case UCI_OPTION_TYPE_STRING:
			fprintf(stream, "string default %s\n", option->data.string.default_val);
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

int
ucioption_cmp(const void *opt1, const void *opt2)
{
	const char *str1 = ((struct UciOption *)(opt1))->name;
	const char *str2 = ((struct UciOption *)(opt2))->name;
	return strncmpci(str1, str2);
}

bool
ucioption_combo_allows(const struct UciOption *option, const char *variant)
{
	const char *pos = strstr(option->data.string.combo_variants, variant);
	return pos > option->data.string.combo_variants && *(pos - 1) == '[' &&
	       *(pos + strlen(variant) + 1) == ']';
}

bool
ucioption_spin_allows(const struct UciOption *option, long val)
{
	return val >= option->data.spin.min && val <= option->data.spin.max;
}

struct UciOption *
ucioption_find(const struct UciOption options[], size_t count, const char *name)
{
	struct UciOption key = { .name = name };
	return (struct UciOption *)(bsearch(
	  &key, options, count, sizeof(struct UciOption), ucioption_cmp));
}
