#include "protocols/support/uci_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
		case UCI_OPTION_TYPE_COMBO:
			fprintf(stream, "combo default %s", option->data.combo.default_val);
			const char *start = option->data.combo.variants;
			while (*start == '[') {
				start++;
				const char *end = strchr(start, ']');
				assert(end);
				fprintf(stream, " var %.*s", (int)(end - start), start);
				start = end + 1;
			}
			assert(!*start);
			fputc('\n', stream);
			break;
		case UCI_OPTION_TYPE_CHECK:
			fprintf(stream,
			        "check default %s\n",
			        option->data.check.default_val ? "true" : "false");
			break;
		case UCI_OPTION_TYPE_STRING:
			fprintf(stream, "string default %s\n", option->data.string.default_val);
			break;
		default:
			exit(EXIT_FAILURE);
	}
}
