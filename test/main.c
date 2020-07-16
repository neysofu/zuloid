#include "engine.h"
#include <stdio.h>
#include <stdarg.h>

// clang-format off
extern void test_attacks(void);
extern void test_cache_single_key_retrieval(void);
extern void test_castling_mask(void);
extern void test_char_to_file(void);
extern void test_char_to_piece(void);
extern void test_color_other(void);
extern void test_fen_conversion(void);
extern void test_fen_init(void);
extern void test_file_to_char(void);
extern void test_piece_to_char(void);
extern void test_position_is_illegal(void);
extern void test_position_is_legal(void);
extern void test_rating(void);
extern void test_perft_results(struct Engine *);
extern void test_uci_cmd_d(struct Engine *);
extern void test_uci_cmd_djbhash(struct Engine *);
extern void test_uci_cmd_isready(struct Engine *);
extern void test_uci_cmd_position(struct Engine *);
extern void test_uci_cmd_quit(struct Engine *);
extern void test_uci_cmd_uci(struct Engine *);
extern void test_uci_unknown_cmd(struct Engine *);
extern void test_utils(void);
// clang-format on

void
call_test(void (*test)(void))
{
	test();
}

void
call_test_with_tmp_engine(void (*test)(struct Engine *))
{
	struct Engine *engine = engine_new_tmp();
	test(engine);
	engine_delete(engine);
}

int
main(void)
{
	init_subsystems();
	call_test(test_utils);
	call_test(test_attacks);
	call_test(test_castling_mask);
	call_test(test_cache_single_key_retrieval);
	call_test(test_char_to_file);
	call_test(test_char_to_piece);
	call_test(test_color_other);
	call_test(test_fen_conversion);
	call_test(test_fen_init);
	call_test(test_file_to_char);
	call_test(test_piece_to_char);
	call_test(test_position_is_illegal);
	call_test(test_position_is_legal);
	call_test(test_rating);
	call_test_with_tmp_engine(test_uci_cmd_d);
	call_test_with_tmp_engine(test_uci_cmd_djbhash);
	call_test_with_tmp_engine(test_uci_cmd_isready);
	call_test_with_tmp_engine(test_uci_cmd_position);
	call_test_with_tmp_engine(test_uci_cmd_quit);
	call_test_with_tmp_engine(test_uci_cmd_uci);
	call_test_with_tmp_engine(test_uci_unknown_cmd);
	//call_test_with_tmp_engine(test_perft_results);
	puts("All tests passed.");
	return EXIT_SUCCESS;
}
