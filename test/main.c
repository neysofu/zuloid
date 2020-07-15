#include "engine.h"
#include <stdio.h>

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
extern void test_uci(struct Engine *);
extern void test_utils(void);
// clang-format on

void
call_with_tmp_engine(void (*test)(struct Engine *))
{
	struct Engine *engine = engine_new_tmp();
	test(engine);
	engine_delete(engine);
}

int
main(void)
{
	init_subsystems();
	test_utils();
	test_attacks();
	test_castling_mask();
	test_cache_single_key_retrieval();
	test_char_to_file();
	test_char_to_piece();
	test_color_other();
	test_fen_conversion();
	test_fen_init();
	test_file_to_char();
	test_piece_to_char();
	test_position_is_illegal();
	test_position_is_legal();
	test_rating();
	call_with_tmp_engine(test_perft_results);
	call_with_tmp_engine(test_uci);
	return EXIT_SUCCESS;
}
