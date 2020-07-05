# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 3.4)

set (CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set (CMAKE_POLICY_DEFAULT_CMP0042 NEW)

project (ZORRO VERSION 1.6.0 LANGUAGES C)

set (CMAKE_C_FLAGS " \
    ${CMAKE_C_FLAGS} \
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

set (default_build_type "Debug")
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -fdata-sections -ffunction-sections -Wl,--gc-sections")

set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set (SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set (TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/")

string (TIMESTAMP BUILD_DATE "%Y-%m-%d")

# base64
add_library (BASE64 "${LIB_DIR}/base64/base64.c")
set_property (TARGET BASE64 PROPERTY C_STANDARD 99)
# cJSON
set (ENABLE_CJSON_TEST OFF)
add_subdirectory ("${LIB_DIR}/cJSON/")
# Mersenne twister
add_library (MT64 "${LIB_DIR}/mt-64/mt19937-64.c")
set_property (TARGET MT64 PROPERTY C_STANDARD 99)
# plibsys
set (PLIBSYS_BUILD_DOC OFF)
add_subdirectory ("${LIB_DIR}/plibsys/")
# xxHash
add_library (XXHASH "${LIB_DIR}/xxHash/xxhash.c")
set_property (TARGET XXHASH PROPERTY C_STANDARD 99)
# Unity
add_library (UNITY "${LIB_DIR}/Unity/src/unity.c")
set_property (TARGET UNITY PROPERTY C_STANDARD 90)

# ZORRO_LIB
file (GLOB_RECURSE SRC_FILES "${INCLUDE_DIR}/*.h" "${SRC_DIR}/*.c")
list (REMOVE_ITEM SRC_FILES "${SRC_DIR}/main.c")
add_library (ZORRO_LIB STATIC "${SRC_FILES}")
target_include_directories (ZORRO_LIB PUBLIC "${INCLUDE_DIR}")
target_include_directories (ZORRO_LIB SYSTEM PUBLIC "${LIB_DIR}")
target_link_libraries (ZORRO_LIB BASE64 cjson MT64 plibsys XXHASH)
target_compile_definitions (ZORRO_LIB PRIVATE
    PROJECT_BUILD_DATE=\"${BUILD_DATE}\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\")

# Zorro
add_executable (ZORRO "${SRC_DIR}/main.c")
target_link_libraries (ZORRO ZORRO_LIB)

# ZORRO_TEST
file (GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.c")
add_executable (ZORRO_TEST "${TEST_FILES}")
target_include_directories (ZORRO_LIB SYSTEM PRIVATE "${LIB_DIR}")
target_link_libraries (ZORRO_TEST ZORRO_LIB UNITY)
target_compile_definitions (ZORRO_TEST PRIVATE
    TEST_RESOURCES=\"${TEST_DIR}/resources/\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\")
add_test (NAME ZORRO COMMAND ZORRO_TEST)
