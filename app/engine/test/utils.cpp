#include <cstdlib>
#include <cstdio>
#include <string>
#include "catch.hpp"
extern "C" {
#include <unistd.h>
#include "utils.h"
}

TEST_CASE("'MAX' works as intended.") {
	CHECK(MAX(42, 1337) == 1337);
	CHECK(MAX(1337, 42) == 1337);
	CHECK(MAX(0, -1) == 0);
}

TEST_CASE("'MIN' works as intended.") {
	CHECK(MIN(42, 1337) == 42);
	CHECK(MIN(1337, 42) == 42);
	CHECK(MIN(-1, 0) == -1);
}

TEST_CASE("'hash_djb2' produces some known collisions.") {
	// From https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
	char str_1a[] = "dram";
	char str_1b[] = "vivency";
	char str_2a[] = "heliotropes";
	char str_2b[] = "neurospora";
	char str_3a[] = "depravement";
	char str_3b[] = "serafins";
	CHECK(hash_djb2(str_1a) == hash_djb2(str_1b));
	CHECK(hash_djb2(str_2a) == hash_djb2(str_2b));
	CHECK(hash_djb2(str_3a) == hash_djb2(str_3b));
}

TEST_CASE("'home_dir' returns an existing directory with read and write privileges.") {
	char *home = home_dir();
	INFO("Z64C sees " << home << " as the home directory.");
	CHECK(access(home, R_OK | W_OK) != -1);
	putenv("HOME=");
	INFO("The environment variable HOME is now unset.");
	INFO("Z64C sees " << home << " as the home directory.");
	CHECK(access(home, R_OK | W_OK) != -1);
}
