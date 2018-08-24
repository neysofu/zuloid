#include <cstdio>
#include <cstdlib>
#include <string>
#include "catch.hpp"
extern "C" {
#include "engine.h"
}

TEST_CASE("'engine_new' allocates memory when necessary.", "[Engine]") {
	struct Engine *engine = engine_new();
	REQUIRE(engine);
	engine_drop(engine);
}
