# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 3.4.1)
project (Z64C LANGUAGES C)
set (CMAKE_C_STANDARD 99)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-missing-braces -Wextra -Wswitch-default -Wswitch-enum -Wunreachable-code -Wunused-macros")
set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")

# Useful macros
string (LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/app/src/" SOURCE_PATH_LENGTH)
add_definitions ("-D__FILE_PREFIX_SIZE__=${SOURCE_PATH_LENGTH}")

add_library (CJSON "${LIB_DIR}/cJSON/cJSON.c")
add_library (XXHASH "${LIB_DIR}/xxHash/xxhash.c")
add_library (MT19937_64 "${LIB_DIR}/mt19937-64/mt19937-64.c")
add_library (UNITY "${LIB_DIR}/Unity/src/unity.c")

# Z64C_LIB
set (Z64C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app/include/")
file (GLOB_RECURSE Z64C_SRC "${Z64C_INCLUDE_DIR}/*.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/app/src/*.c")
list (REMOVE_ITEM Z64C_SRC "${CMAKE_CURRENT_SOURCE_DIR}/app/src/main.c")
add_library (Z64C_LIB STATIC ${Z64C_SRC})
target_include_directories (Z64C_LIB PUBLIC ${LIB_DIR})
target_include_directories (Z64C_LIB PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_LIB CJSON)
target_link_libraries (Z64C_LIB XXHASH)
target_link_libraries (Z64C_LIB MT19937_64)

# Z64C
add_executable (Z64C ${Z64C_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/app/src/main.c")
target_link_libraries (Z64C Z64C_LIB)

# Z64C_TEST
file (GLOB_RECURSE Z64C_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/app/test/*.c")
add_executable (Z64C_TEST ${Z64C_TEST_SRC})
target_include_directories (Z64C_TEST PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_TEST Z64C_LIB)
target_link_libraries (Z64C_TEST UNITY)
enable_testing ()
add_test (NAME Z64C COMMAND Z64C_TEST)
