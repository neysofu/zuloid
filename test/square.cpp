#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "square.h"
}

const char square_1_as_str = 'K';
const char square_2_as_str = '.';
const char square_3_as_str = 'p';

TEST_CASE("Going back and forth between 'square_to_char' and 'char_to_square' doesn't alter it.", "[Square]") {
}
