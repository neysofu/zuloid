#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "engine.h"
}

TEST_CASE("'engine_new' allocates memory when necessary.", "[Engine]") {
	struct Engine *engine = engine_new(NULL);
	REQUIRE(engine);
	engine_drop(engine);
}

TEST_CASE("'engine_new' is idempotent.", "[Engine]") {
	struct Engine *engine = engine_new(NULL);
	REQUIRE(engine == engine_new(engine));
	engine_drop(engine);
}
