#include "engine.h"
#include "munit/munit.h"
#include "chess/position.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_uci_cmd_position(struct Engine *engine)
{
	Square e4 = square_new(char_to_file('e'), char_to_rank('4'));
	char cmd[] = "position current moves e2e4";
	struct Piece piece_at_e4 = position_piece_at_square(&engine->board, e4);
	munit_assert_uint(piece_at_e4.type, !=, PIECE_TYPE_PAWN);
	protocol_uci_handle(engine, cmd);
	struct Lines *lines = read_last_lines(engine->output);
	munit_assert_uint(lines_count(lines), ==, 0);
	piece_at_e4 = position_piece_at_square(&engine->board, e4);
	munit_assert_uint(piece_at_e4.type, ==, PIECE_TYPE_PAWN);
	lines_delete(lines);
}
