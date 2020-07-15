#include "engine.h"
#include "chess/fen.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_uci_cmd_d(struct Engine *engine)
{
	char cmd[] = "d";
	protocol_uci_handle(engine, cmd);
	struct Lines *lines = read_last_lines(engine->output);
	size_t n_lines = lines_count(lines);
	char *fen = exit_if_null(malloc(FEN_SIZE));
	fen_from_position(fen, &POSITION_INIT, ' ');
	munit_assert_uint(n_lines, >, FILES_COUNT);
	bool fen_was_found = false;
	for (size_t i = 0; i < n_lines; i++) {
		fen_was_found = fen_was_found || strstr(lines_nth(lines, i), fen);
	}
	munit_assert_true(fen_was_found);
	free(fen);
}
