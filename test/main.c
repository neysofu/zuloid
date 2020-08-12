#include "chess/diagonals.h"
#include "test/utils.h"
#include <stdarg.h>
#include <stdio.h>

#define STRINGIFY(x) #x

#define CALL_TEST(f)                                                                       \
	do {                                                                                   \
		test_pre(STRINGIFY(f));                                                            \
		(f)();                                                                             \
	} while (0)

#define CALL_TEST_WITH_ARGS(f, ...)                                                        \
	do {                                                                                   \
		test_pre(STRINGIFY(f));                                                            \
		(f)(__VA_ARGS__);                                                                  \
	} while (0)

#define CALL_TEST_WITH_TMP_ENGINE(f)                                                       \
	do {                                                                                   \
		test_pre(STRINGIFY(f));                                                            \
		struct Engine *engine = engine_new_tmp(TEST_TMP_DIR "/tmp");                       \
		(f)(engine);                                                                       \
		engine_delete(engine);                                                             \
	} while (0)

void
test_pre(const char *function_name)
{
	printf("-- %s\n", function_name);
}

// clang-format off
extern void test_960(void);
extern void test_bb_subset(void);
extern void test_attacks(void);
extern void test_cache_single_key_retrieval(void);
extern void test_castling_mask(void);
extern void test_char_to_file(void);
extern void test_char_to_piece(void);
extern void test_color_other(void);
extern void test_diagonals_dont_overlap(const Bitboard diagonals[15]);
extern void test_fen_conversion(void);
extern void test_fen_init(void);
extern void test_file_to_char(void);
extern void test_init(void);
extern void test_magic_generation(void);
extern void test_piece_to_char(void);
extern void test_position_is_illegal(void);
extern void test_position_is_legal(void);
extern void test_rating(void);
extern void test_perft_results(struct Engine *);
extern void test_protocol_cecp(struct Engine *);
extern void test_protocol_cecp_ping(struct Engine *);
extern void test_protocol_cecp_quit(struct Engine *);
extern void test_protocol_switch(struct Engine *);
extern void test_protocol_uci_empty(struct Engine *);
extern void test_protocol_uci_cmd_d(struct Engine *);
extern void test_protocol_uci_cmd_debug(struct Engine *);
extern void test_protocol_uci_cmd_djbhash(struct Engine *);
extern void test_protocol_uci_cmd_go_perft(struct Engine *);
extern void test_protocol_uci_cmd_isready(struct Engine *);
extern void test_protocol_uci_cmd_position(struct Engine *);
extern void test_protocol_uci_cmd_quit(struct Engine *);
extern void test_protocol_uci_cmd_uci(struct Engine *);
extern void test_protocol_uci_unknown_cmd(struct Engine *);
extern void test_utils(void);
// clang-format on

int
main(void)
{
	init_subsystems();
	CALL_TEST(test_utils);
	CALL_TEST(test_960);
	CALL_TEST(test_attacks);
	CALL_TEST(test_bb_subset);
	CALL_TEST(test_castling_mask);
	CALL_TEST(test_cache_single_key_retrieval);
	CALL_TEST(test_char_to_file);
	CALL_TEST(test_char_to_piece);
	CALL_TEST(test_color_other);
	CALL_TEST_WITH_ARGS(test_diagonals_dont_overlap, DIAGONALS_A1H8);
	CALL_TEST_WITH_ARGS(test_diagonals_dont_overlap, DIAGONALS_A8H1);
	CALL_TEST(test_fen_conversion);
	CALL_TEST(test_fen_init);
	CALL_TEST(test_file_to_char);
	CALL_TEST(test_init);
	CALL_TEST(test_magic_generation);
	CALL_TEST(test_piece_to_char);
	CALL_TEST(test_position_is_illegal);
	CALL_TEST(test_position_is_legal);
	CALL_TEST(test_rating);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_cecp);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_cecp_ping);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_cecp_quit);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_switch);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_empty);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_d);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_debug);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_djbhash);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_go_perft);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_isready);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_position);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_quit);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_cmd_uci);
	CALL_TEST_WITH_TMP_ENGINE(test_protocol_uci_unknown_cmd);
	CALL_TEST_WITH_TMP_ENGINE(test_perft_results);
	puts("All tests passed.");
	return EXIT_SUCCESS;
}
