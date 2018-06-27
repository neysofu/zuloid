#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "utils.h"
}

TEST_CASE("'MAX' works as intended.") {
	REQUIRE(MAX(42, 1337) == 1337);
	REQUIRE(MAX(1337, 42) == 1337);
	REQUIRE(MAX(0, 0) == 0);
}

TEST_CASE("'MIN' works as intended.") {
	REQUIRE(MIN(42, 1337) == 42);
	REQUIRE(MIN(1337, 42) == 42);
	REQUIRE(MIN(0, 0) == 0);
}

TEST_CASE("'hash' produces some known collisions for DJB2.") {
	// From https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
	char str_1a[] = "dram";
	char str_1b[] = "vivency";
	char str_2a[] = "heliotropes";
	char str_2b[] = "neurospora";
	char str_3a[] = "depravement";
	char str_3b[] = "serafins";
	REQUIRE(hash_djb2(str_1a) == hash_djb2(str_1b));
	REQUIRE(hash_djb2(str_2a) == hash_djb2(str_2b));
	REQUIRE(hash_djb2(str_3a) == hash_djb2(str_3b));
}
