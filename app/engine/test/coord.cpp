#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "coord.h"
#include "board.h"
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
	CHECK(coord_dir(coord_new(0, 0), coord_new(2, 1)) == DIR_L_SHAPED);
	CHECK(coord_dir(coord_new(0, 0), coord_new(1, 2)) == DIR_L_SHAPED);
}

TEST_CASE("'coord_dir' recognizes horizontal movement.", "[Coord]") {
	CHECK(coord_dir(coord_new(0, 0), coord_new(7, 0)) == DIR_HORIZONTAL);
}

TEST_CASE("'coord_dir' recognizes vertical movement.", "[Coord]") {
	CHECK(coord_dir(coord_new(0, 0), coord_new(0, 7)) == DIR_VERTICAL);
}

TEST_CASE("'coord_dir' recognizes diagonal movement.", "[Coord]") {
	CHECK(coord_dir(coord_new(0, 0), coord_new(7, 7)) == DIR_DIAGONAL);
	CHECK(coord_dir(coord_new(0, 7), coord_new(7, 0)) == DIR_DIAGONAL);
}

TEST_CASE("'coord_dir' shows no direction for 'COORD_NONE'.", "[Coord]") {
	CHECK(coord_dir(COORD_NONE, coord_new(2, 5)) == DIR_NONE);
}
