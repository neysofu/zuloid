#include "Unity/src/unity.h"

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
// clang-format on

int
main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_attacks);
	RUN_TEST(test_cache_single_key_retrieval);
	RUN_TEST(test_castling_mask);
	RUN_TEST(test_char_to_file);
	RUN_TEST(test_char_to_piece);
	RUN_TEST(test_color_other);
	RUN_TEST(test_fen_conversion);
	RUN_TEST(test_fen_init);
	RUN_TEST(test_file_to_char);
	RUN_TEST(test_piece_to_char);
	RUN_TEST(test_position_is_illegal);
	RUN_TEST(test_position_is_legal);
	RUN_TEST(test_rating);
	return UNITY_END();
}
