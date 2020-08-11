#include "chess/bb.h"
#include "chess/find_magics.h"
#include "utils.h"
#include <assert.h>
#include <inttypes.h>
#include <libpopcnt/libpopcnt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANDLE_ERR(err) do { if (err < 0) { return -1; } } while (0)

int
magics_export(const struct Magic *magics, const char *identifier, FILE *stream)
{
	int err;
	err = fprintf(stream,
	        "#include \"chess/coordinates.h\"\n"
	        "#include \"chess/find_magics.h\"\n"
	        "\n"
	        "const struct Magic %s[SQUARES_COUNT] = {\n",
	        identifier);
	HANDLE_ERR(err);
	for (Square sq = 0; sq < SQUARES_COUNT; sq++) {
		struct Magic magic = magics[sq];
		err = fprintf(stream,
		        "\t[0%o] = { .premask = 0x%" PRIx64 "ULL, .multiplier = 0x%" PRIx64
		        "ULL, .rshift = %d, "
		        ".postmask = "
		        "0x%" PRIx64 "ULL, .start = %zu, .end = %zu },\n",
		        sq,
		        magic.premask,
		        magic.multiplier,
		        magic.rshift,
		        magic.postmask,
		        magic.start,
		        magic.end);
		HANDLE_ERR(err);
	}
	fprintf(stream, "};\n");
	HANDLE_ERR(err);
	return 0;
}
