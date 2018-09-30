#include <cstdlib>
#include <cstdio>
#include <string>
#include "Catch2/single_include/catch.hpp"
extern "C" {
#include <time.h>
#include <unistd.h>
#include "utils.h"
}

TEST_CASE("'MAX' works as intended.") {
	CHECK(MAX(42, 1337) == 1337);
	CHECK(MAX(1337, 42) == 1337);
	CHECK(MAX(-1337, 42) == 42);
	CHECK(MAX(-2, -2) == -2);
}

TEST_CASE("'MIN' works as intended.") {
	CHECK(MIN(42, 1337) == 42);
	CHECK(MIN(1337, 42) == 42);
	CHECK(MIN(-1, 0) == -1);
	CHECK(MIN(3, 3) == 3);
}

TEST_CASE("'util_home_dir' returns an existing directory with read and write privileges.") {
	char *home = util_home_dir();
	INFO("Z64C sees " << home << " as the home directory.");
	CHECK(access(home, R_OK | W_OK) != -1);
	putenv("HOME=");
	INFO("The environment variable HOME is now unset.");
	INFO("Z64C sees " << home << " as the home directory.");
	CHECK(access(home, R_OK | W_OK) != -1);
}

TEST_CASE("'util_timestamp_msec'.") {
	CHECK(MAX(time() - util_timestamp_msec() / 1000, 1) == 1);
}

TEST_CASE("'util_is_stdin_empty' works.") {
	FILE *stream =
	fflush(stream);
	fprintf(file, "");
	REQUIRE(util_is_stream_empty(file));
	fprintf(file, "foobar\n");
	REQUIRE(!util_is_stream_empty(file));
}
