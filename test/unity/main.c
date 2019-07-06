/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "Unity/src/unity.h"

extern void
test_cache_single_key_retrieval(void);
extern void
test_castling_mask(void);
extern void
test_char_to_file(void);
extern void
test_char_to_piece(void);
extern void
test_color(void);
extern void
test_fen_conversion(void);
extern void
test_fen_init(void);
extern void
test_file_to_char(void);
extern void
test_jsonrpc_compliance(void);
extern void
test_piece_to_char(void);
extern void
test_position_is_illegal(void);
extern void
test_position_is_legal(void);

int
main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_cache_single_key_retrieval);
	RUN_TEST(test_castling_mask);
	RUN_TEST(test_char_to_file);
	RUN_TEST(test_char_to_piece);
	RUN_TEST(test_color);
	RUN_TEST(test_fen_conversion);
	RUN_TEST(test_fen_init);
	RUN_TEST(test_file_to_char);
	RUN_TEST(test_jsonrpc_compliance);
	RUN_TEST(test_piece_to_char);
	RUN_TEST(test_position_is_illegal);
	RUN_TEST(test_position_is_legal);
	return UNITY_END();
}
