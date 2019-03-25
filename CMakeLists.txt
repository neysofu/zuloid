# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 3.4)

set (CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set (CMAKE_POLICY_DEFAULT_CMP0042 NEW)

project (Z64C VERSION 1.1.0 LANGUAGES C)

set (CMAKE_C_FLAGS_DEBUG "-g -ftrapv")
set (CMAKE_C_FLAGS_RELEASE "-O3")
set (CMAKE_C_FLAGS " \
	-fverbose-asm \
	-Wall \
	-Wpedantic \
	-Wextra \
	-Wshadow \
	-Wno-missing-braces \
	-Wno-missing-field-initializers \
	-Wswitch-default \
	-Wcast-align \
	-Wpointer-arith \
	-Wundef \
	-Wuninitialized \
	-Wredundant-decls \
	-Wold-style-definition \
	-Wunreachable-code \
	-Wunused-macros")

set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set (SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set (TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/")

string (LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" PROJECT_DIR_LENGTH)
string (TIMESTAMP BUILD_DATE "%Y-%m-%d")

find_package (OpenCL REQUIRED)

# base64.c
add_library (BASE64C "${LIB_DIR}/base64.c/base64.c")
set_property (TARGET BASE64C PROPERTY C_STANDARD 99)
# cJSON
set (ENABLE_CJSON_TEST OFF)
add_subdirectory ("${LIB_DIR}/cJSON/")
# xxHash
add_library (XXHASH "${LIB_DIR}/xxHash/xxhash.c")
set_property (TARGET XXHASH PROPERTY C_STANDARD 90)
# Unity
add_library (UNITY "${LIB_DIR}/Unity/src/unity.c")
set_property (TARGET UNITY PROPERTY C_STANDARD 90)
# TicToc
add_library (TICTOC "${LIB_DIR}/tictoc/src/tictoc.c")
set_property (TARGET TICTOC PROPERTY C_STANDARD 99)

# Z64C_LIB
file (GLOB_RECURSE SRC_FILES "${INCLUDE_DIR}/*.h" "${SRC_DIR}/*.c")
list (REMOVE_ITEM SRC_FILES "${SRC_DIR}/main.c")
add_library (Z64C_LIB STATIC "${SRC_FILES}")
target_include_directories (Z64C_LIB PUBLIC "${INCLUDE_DIR}" PUBLIC "${LIB_DIR}")
target_link_libraries (Z64C_LIB BASE64C cjson XXHASH TICTOC)
target_compile_definitions (Z64C_LIB PUBLIC
	PROJECT_DIR_LENGTH=${PROJECT_DIR_LENGTH}
	BUILD_DATE=\"${BUILD_DATE}\"
	Z64C_VERSION_MAJOR=\"${PROJECT_VERSION_MAJOR}\"
	Z64C_VERSION_MINOR=\"${PROJECT_VERSION_MINOR}\"
	Z64C_VERSION_PATCH=\"${PROJECT_VERSION_PATCH}\")

# Z64C
add_executable (Z64C "${SRC_DIR}/main.c")
target_link_libraries (Z64C Z64C_LIB)

# Z64C_TEST
file (GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.c")
add_executable (Z64C_TEST "${TEST_FILES}")
target_include_directories (Z64C_TEST PRIVATE "${INCLUDE_DIR}")
target_link_libraries (Z64C_TEST Z64C_LIB UNITY)
add_test (NAME Z64C COMMAND Z64C_TEST)
