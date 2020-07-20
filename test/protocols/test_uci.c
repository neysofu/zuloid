#include "engine.h"
#include "chess/fen.h"
#include "munit/munit.h"
#include "protocols/uci.h"
#include "test/utils.h"
#include "utils.h"

void
test_uci_commands_are_sorted(void) {
	for (size_t i = 0; i < UCI_COMMANDS_COUNT - 1; i++) {
		int result = uci_command_cmp(UCI_COMMANDS + i, UCI_COMMANDS + i + 1);
		munit_assert_int(result, <, 0);
	}
}

void
test_uci_empty(struct Engine *engine) {
	protocol_uci_handle(engine, "   ");
	protocol_uci_handle(engine, "");
	protocol_uci_handle(engine, "\n \t ");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_uint(lines_count(lines), ==, 0);
}

void
test_uci_cmd_d(struct Engine *engine)
{
	protocol_uci_handle(engine, "d lichess");
	protocol_uci_handle(engine, "d");
	struct Lines *lines = file_line_by_line(engine->output);
	size_t n_lines = lines_count(lines);
	char *fen = exit_if_null(malloc(FEN_SIZE));
	fen_from_position(fen, &POSITION_INIT, ' ');
	munit_assert_uint(n_lines, >, FILES_COUNT);
	munit_assert_not_null(strstr(lines_nth(lines, 0), "lichess.org"));
	bool fen_was_found = false;
	for (size_t i = 1; i < n_lines; i++) {
		fen_was_found = fen_was_found || strstr(lines_nth(lines, i), fen);
	}
	munit_assert_true(fen_was_found);
	free(fen);
}

void
test_uci_cmd_go_perft(struct Engine *engine)
{
	protocol_uci_handle(engine, "go perft");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_not_null(strstr(lines_nth(lines, 0), "ERROR"));
	lines_delete(lines);
}

void
test_uci_cmd_djbhash(struct Engine *engine)
{
	protocol_uci_handle(engine, "djbhash position");
	protocol_uci_handle(engine, "djbhash");
	protocol_uci_handle(engine, "djbhash foobar foobar");
	protocol_uci_handle(engine, "djbhash foobar position foobar");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_uint(atoi(lines_nth(lines, 0)), ==, 31418);
	munit_assert_uint(atoi(lines_nth(lines, 1)), ==, 0);
	munit_assert_uint(atoi(lines_nth(lines, 2)), ==, 0);
	munit_assert_uint(atoi(lines_nth(lines, 3)), ==, 31418);
	lines_delete(lines);
}

void
test_uci_cmd_isready(struct Engine *engine)
{
	protocol_uci_handle(engine, "isready");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_string_equal(lines_nth(lines, 0), "readyok");
	lines_delete(lines);
}

void
test_uci_cmd_position(struct Engine *engine)
{
	Square e4 = square_new(char_to_file('e'), char_to_rank('4'));
	struct Piece piece_at_e4 = position_piece_at_square(&engine->board, e4);
	munit_assert_uint(piece_at_e4.type, !=, PIECE_TYPE_PAWN);

	protocol_uci_handle(engine, "position current moves e2e4");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_uint(lines_count(lines), ==, 0);
	piece_at_e4 = position_piece_at_square(&engine->board, e4);
	munit_assert_uint(piece_at_e4.type, ==, PIECE_TYPE_PAWN);

	protocol_uci_handle(engine, "position startpos");
	lines = file_line_by_line(engine->output);
	piece_at_e4 = position_piece_at_square(&engine->board, e4);
	munit_assert_uint(piece_at_e4.type, ==, PIECE_TYPE_NONE);

	lines_delete(lines);
}

void
test_uci_cmd_quit(struct Engine *engine)
{
	munit_assert_uint(engine->status, ==, STATUS_IDLE);
	protocol_uci_handle(engine, "quit");
	munit_assert_uint(engine->status, ==, STATUS_EXIT);
}

void
test_uci_cmd_uci(struct Engine *engine)
{
	protocol_uci_handle(engine, "uci");
	struct Lines *lines = file_line_by_line(engine->output);
	for (size_t i = 0; i < lines_count(lines) - 1; i++) {
		char *first_token = strtok_whitespace(lines_nth(lines, i));
		munit_assert_true(strcmp(first_token, "id") == 0 || strcmp(first_token, "option") == 0);
	}
	munit_assert_string_equal(lines_nth(lines, -1), "uciok");
	lines_delete(lines);
}

void
test_uci_unknown_cmd(struct Engine *engine)
{
	protocol_uci_handle(engine, "foobar");
	struct Lines *lines = file_line_by_line(engine->output);
	munit_assert_uint(lines_count(lines), ==, 1);
	munit_assert_true(strstr(lines_nth(lines, 0), "ERROR") != NULL);
	lines_delete(lines);
}
