#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "square.h"
}

const char square_1_as_char = 'K';
const char square_2_as_char = '.';
const char square_3_as_char = 'p';
const char square_4_as_char = 'r';
const char square_5_as_char = 'N';

TEST_CASE("'square_to_char' after 'char_to_square' is idempotent.", "[Square]") {
	REQUIRE(square_to_char(char_to_square(square_1_as_char)) == square_1_as_char);
	REQUIRE(square_to_char(char_to_square(square_2_as_char)) == square_2_as_char);
	REQUIRE(square_to_char(char_to_square(square_3_as_char)) == square_3_as_char);
	REQUIRE(square_to_char(char_to_square(square_4_as_char)) == square_4_as_char);
	REQUIRE(square_to_char(char_to_square(square_5_as_char)) == square_5_as_char);
}
