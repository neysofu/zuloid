cmake_minimum_required(VERSION 3.4)

set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0042 NEW)

project(ZORRO VERSION 1.6.0 LANGUAGES C)

set(CMAKE_C_FLAGS " \
   ${CMAKE_C_FLAGS} \
   -fverbose-asm \
   -save-temps \
   -Wall \
   -Wpedantic \
   -Wextra \
   -Wshadow \
   -Wcomment \
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

set(default_build_type "Debug")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -fdata-sections -ffunction-sections -Wl,--gc-sections")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/")

string(TIMESTAMP BUILD_DATE "%Y-%m-%d")

# Dependencies:
# -----------------
# base64
add_library(BASE64 "${LIB_DIR}/base64/base64.c")
set_property(TARGET BASE64 PROPERTY C_STANDARD 99)
# cJSON
set(ENABLE_CJSON_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIB_DIR}/cJSON/")
# Mersenne twister
add_library(MT64 "${LIB_DIR}/mt-64/mt19937-64.c")
set_property(TARGET MT64 PROPERTY C_STANDARD 99)
# plibsys
set(PLIBSYS_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(PLIBSYS_TESTS OFF CACHE BOOL "" FORCE)
set(PLIBSYS_BUILD_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIB_DIR}/plibsys/")
# xxHash
add_library(XXHASH "${LIB_DIR}/xxHash/xxhash.c")
set_property(TARGET XXHASH PROPERTY C_STANDARD 99)
# munit
add_library(munit "${LIB_DIR}/munit/munit.c")
set_property(TARGET munit PROPERTY C_STANDARD 99)

set_target_properties(BASE64 cjson MT64 plibsys XXHASH munit PROPERTIES COMPILE_FLAGS "-w")

# ZORRO_LIB
file(GLOB_RECURSE SRC_FILES "${INCLUDE_DIR}/*.h" "${SRC_DIR}/*.c")
list(REMOVE_ITEM SRC_FILES "${SRC_DIR}/main.c")
add_library(ZORRO_LIB STATIC "${SRC_FILES}")
target_include_directories(ZORRO_LIB PUBLIC "${INCLUDE_DIR}")
target_include_directories(ZORRO_LIB SYSTEM PUBLIC "${LIB_DIR}")
target_link_libraries(ZORRO_LIB BASE64 cjson MT64 plibsys XXHASH)
target_compile_definitions(ZORRO_LIB PRIVATE
    PROJECT_BUILD_DATE=\"${BUILD_DATE}\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\")

# Zorro
add_executable(zorro "${SRC_DIR}/main.c")
target_link_libraries(zorro ZORRO_LIB)

# Tests
file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.c")
add_executable(test_zorro "${TEST_FILES}")
target_include_directories(ZORRO_LIB SYSTEM PRIVATE "${LIB_DIR}")
target_link_libraries(test_zorro ZORRO_LIB munit)
target_compile_definitions(test_zorro PRIVATE
    TEST_RESOURCES=\"${TEST_DIR}/resources/\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\")
add_test(NAME ZORRO COMMAND test_zorro)

add_custom_target(run
    COMMAND zorro
    DEPENDS zorro
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
