# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 3.4.1)
project (Z64C VERSION 0.10.0 LANGUAGES C)
set (CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE DEBUG)
endif ()

set (CMAKE_C_FLAGS_DEBUG "-g -fverbose-asm -finstrument-functions -ftrapv -fprofile-generate")
set (CMAKE_C_FLAGS_RELEASE "-O3")

# Custom compiler settings
set (CMAKE_C_FLAGS " \
	 -Wall \
	 -Wpedantic \
	 -Wextra \
	 -Wshadow \
	 -Wno-missing-braces \
	 -Wno-missing-field-initializers \
	 -Wswitch-default \
	 -Wswitch-enum \
	 -Wcast-align \
	 -Wpointer-arith \
	 -Wundef \
	 -Wuninitialized \
	 -Wredundant-decls \
	 -Wold-style-definition \
	 -Wunreachable-code \
	 -Wunused-macros")

string (TIMESTAMP TIMESTAMP "%Y-%m-%d")
string (LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/src/" SRC_PATH_LENGTH)
add_definitions ("-D__TIMESTAMP_ISO_8601__=\"${TIMESTAMP}\"")
add_definitions ("-DSRC_PATH_LENGTH=${SRC_PATH_LENGTH}")

# Dependencies
set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set (ENABLE_CJSON_TEST OFF)
add_subdirectory ("${LIB_DIR}/cJSON/")
add_library (UNITY "${LIB_DIR}/Unity/src/unity.c")
add_library (XXHASH "${LIB_DIR}/xxHash/xxhash.c")
target_include_directories (XXHASH PUBLIC "${LIB_DIR}/xxHash/")

# Z64C_LIB
set (Z64C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
file (GLOB_RECURSE Z64C_SRC "${Z64C_INCLUDE_DIR}/*.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list (REMOVE_ITEM Z64C_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
add_library (Z64C_LIB STATIC ${Z64C_SRC})
target_include_directories (Z64C_LIB PUBLIC ${LIB_DIR})
target_include_directories (Z64C_LIB PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_LIB cjson)
target_link_libraries (Z64C_LIB XXHASH)
target_compile_definitions (Z64C_LIB PRIVATE Z64C_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
	Z64C_VERSION_MINOR=${PROJECT_VERSION_MINOR}
	Z64C_VERSION_PATCH=${PROJECT_VERSION_PATCH})

# Z64C
add_executable (Z64C ${Z64C_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
target_link_libraries (Z64C Z64C_LIB)

# Z64C_TEST
file (GLOB_RECURSE Z64C_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")
add_executable (Z64C_TEST ${Z64C_TEST_SRC})
target_include_directories (Z64C_TEST PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_TEST Z64C_LIB)
target_link_libraries (Z64C_TEST UNITY)
enable_testing ()
add_test (NAME Z64C COMMAND Z64C_TEST)

set_target_properties (Z64C_LIB Z64C Z64C_TEST PROPERTIES C_STANDARD 99)
