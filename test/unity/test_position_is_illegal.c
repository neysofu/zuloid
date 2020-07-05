/* Copyright (c) Filippo Costa - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * PROPRIETARY AND CONFIDENTIAL */

#include "Unity/src/unity.h"
#include "chess/fen.h"
#include "chess/movegen.h"
#include "utils.h"

void
test_position_is_legal(void)
{
	struct Board pos = POSITION_INIT;
	FILE *file = fopen(TEST_RESOURCES "/legal_positions/legal.epd", "r");
	size_t fen_size = FEN_SIZE;
	char *fen = malloc(fen_size);
	if (!file) {
		return;
	}
	//while (read_line(&fen, &fen_size, file) >= 0) {
	//	position_init_from_fen(&pos, fen);
	//	TEST_ASSERT_FALSE_MESSAGE(position_is_illegal(&pos), fen);
	//}
	fclose(file);
	free(fen);
}

void
test_position_is_illegal(void)
{
	struct Board pos = POSITION_INIT;
	FILE *file = fopen(TEST_RESOURCES "/illegal_positions/illegal.epd", "r");
	size_t fen_size = FEN_SIZE;
	char *fen = malloc(fen_size);
	if (!file) {
		return;
	}
	//while (read_line(&fen, &fen_size, file) >= 0) {
	//	position_init_from_fen(&pos, fen);
	//	TEST_ASSERT_MESSAGE(position_is_illegal(&pos), fen);
	//}
	fclose(file);
	free(fen);
}
