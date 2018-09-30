#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Catch2/single_include/catch.hpp"
extern "C" {
#include "chess/board.h"
#include "chess/coord.h"
#include "chess/dir.h"
}

const char coord_1_as_str[] = "a1";
const char coord_2_as_str[] = "e2";
const char coord_3_as_str[] = "f8";

TEST_CASE("Going back and forth between 'str_to_coord' and 'coord_to_str' doesn't alter coordinates.", "[Coord]") {
	//Coord coord = str_to_coord(&coord_1_as_str);
	//char *str = coord_to_str(coord, NULL);
	//CHECK(strcmp(str, coord_1_as_str) == 0);
	//coord = str_to_coord(&coord_2_as_str);
	//str = coord_to_str(coord, str);
	//CHECK(strcmp(str, coord_2_as_str) == 0);
	//coord = str_to_coord(&coord_3_as_str);
	//str = coord_to_str(coord, str);
	//CHECK(strcmp(str, coord_3_as_str) == 0);
	//free(str);
}

TEST_CASE("'coord_dir' recognizes L-shaped movement.", "[Coord]") {
	CHECK(dir_from_to(coord_new(0, 0), coord_new(2, 1)) == DIR_L_SHAPED);
	CHECK(dir_from_to(coord_new(0, 0), coord_new(1, 2)) == DIR_L_SHAPED);
}

TEST_CASE("") {
	size_t moves_counter_by_dir[DIR_NONE] = {0};
	for (Coord coord_0 = BOARD_NUM_SQUARES; coord_0 > 0; coord_0--) {
		for (Coord coord_1 = BOARD_NUM_SQUARES; coord_1 > 0; coord_1--) {
			switch (dir_from_to(coord_0, coord_1)) {
				case DIR_HORIZONTAL:
					moves_counter_by_dir[DIR_HORIZONTAL]++;
					break;
				case DIR_VERTICAL:
					moves_counter_by_dir[DIR_VERTICAL]++;
					break;
				case DIR_DIAGONAL:
					moves_counter_by_dir[DIR_DIAGONAL]++;
					break;
				case DIR_L_SHAPED:
					moves_counter_by_dir[DIR_L_SHAPED]++;
					break;
				case DIR_NONE:
					moves_counter_by_dir[DIR_NONE]++;
					break;
				default:
					assert(false);
			}
		}
	}
	CHECK(moves_counter_by_dir[DIR_HORIZONTAL] == 8 * 64);
	CHECK(moves_counter_by_dir[DIR_VERTICAL] == 7 * 64);
	CHECK(moves_counter_by_dir[DIR_DIAGONAL] == 7 * 28 +
												9 * 20 +
												11 * 12 +
												13 * 4);
	CHECK(moves_counter_by_dir[DIR_L_SHAPED] == 4 * 2 +
											    8 * 3 +
											    20 * 4 +
											    16 * 6 +
											    8 * 16);
	size_t moves_with_dir_counter = moves_counter_by_dir[DIR_HORIZONTAL] +
									moves_counter_by_dir[DIR_VERTICAL] +
									moves_counter_by_dir[DIR_DIAGONAL] +
									moves_counter_by_dir[DIR_NONE];
	CHECK(moves_counter_by_dir[DIR_NONE] == 64 * 64 - moves_with_dir_counter);
}

TEST_CASE("'coord_dir' recognizes horizontal movement.", "[Coord]") {
	CHECK(dir_from_to(coord_new(0, 0), coord_new(7, 0)) == DIR_HORIZONTAL);
}

TEST_CASE("'coord_dir' recognizes vertical movement.", "[Coord]") {
	CHECK(dir_from_to(coord_new(0, 0), coord_new(0, 7)) == DIR_VERTICAL);
}

TEST_CASE("'coord_dir' recognizes diagonal movement.", "[Coord]") {
	CHECK(dir_from_to(coord_new(0, 0), coord_new(7, 7)) == DIR_DIAGONAL);
	CHECK(dir_from_to(coord_new(0, 7), coord_new(7, 0)) == DIR_DIAGONAL);
}

TEST_CASE("'coord_dir' shows no direction for 'COORD_NONE'.", "[Coord]") {
	CHECK(dir_from_to(COORD_NONE, coord_new(2, 5)) == DIR_NONE);
}
