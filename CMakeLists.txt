cmake_minimum_required(VERSION 3.4)

set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0042 NEW)

project(ZULOID VERSION 0.1 LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
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

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(TEST_TMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-tmp")

file(MAKE_DIRECTORY "${TEST_TMP_DIR}")
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")

# Dependencies:
# -----------------
# base64
add_library(base64 "${LIB_DIR}/base64/base64.c")
set_property(TARGET base64 PROPERTY C_STANDARD 99)
# cJSON
set(ENABLE_CJSON_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIB_DIR}/cJSON")
# Mersenne twister
add_library(mt64 "${LIB_DIR}/mt-64/mt19937-64.c")
set_property(TARGET mt64 PROPERTY C_STANDARD 99)
# plibsys
set(PLIBSYS_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(PLIBSYS_TESTS OFF CACHE BOOL "" FORCE)
set(PLIBSYS_BUILD_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory("${LIB_DIR}/plibsys")
# xxHash
add_library(xxhash "${LIB_DIR}/xxHash/xxhash.c")
set_property(TARGET xxhash PROPERTY C_STANDARD 99)
# munit
add_library(munit "${LIB_DIR}/munit/munit.c")
set_property(TARGET munit PROPERTY C_STANDARD 99)

set_target_properties(base64 cjson mt64 plibsys xxhash munit PROPERTIES COMPILE_FLAGS "-w")

# ZULOID_LIB
file(GLOB_RECURSE SRC_FILES "${INCLUDE_DIR}/*.h" "${SRC_DIR}/*.c")
list(REMOVE_ITEM SRC_FILES "${SRC_DIR}/main.c")
add_library(ZULOID_LIB STATIC "${SRC_FILES}")
target_include_directories(ZULOID_LIB PUBLIC "${INCLUDE_DIR}")
target_include_directories(ZULOID_LIB SYSTEM PUBLIC "${LIB_DIR}")
target_link_libraries(ZULOID_LIB base64 cjson mt64 plibsys xxhash)
target_compile_definitions(ZULOID_LIB PRIVATE
    PROJECT_BUILD_DATE=\"${BUILD_DATE}\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\")

# Zuloid
add_executable(zuloid "${SRC_DIR}/main.c")
target_link_libraries(zuloid ZULOID_LIB)

# Tests
enable_testing()
file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.c")
add_executable(test_zuloid "${TEST_FILES}")
target_include_directories(ZULOID_LIB PUBLIC "${TEST_DIR}")
target_include_directories(ZULOID_LIB SYSTEM PRIVATE "${LIB_DIR}")
target_link_libraries(test_zuloid ZULOID_LIB munit)
target_compile_definitions(test_zuloid PRIVATE
    TEST_RESOURCES=\"${TEST_DIR}/resources\"
    PROJECT_VERSION=\"${PROJECT_VERSION}\"
    TEST_TMP_DIR=\"${TEST_TMP_DIR}\")
add_test(NAME ZULOID COMMAND test_zuloid)

function(zuloid_compiler_flag FLAG)
    set_target_properties(ZULOID_LIB zuloid test_zuloid PROPERTIES COMPILE_FLAGS "${FLAG}")
endfunction()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    zuloid_compiler_flag("-fcolor-diagnostics")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    zuloid_compiler_flag("-fdiagnostics-color=always")
endif()

add_custom_target(run
    COMMAND zuloid
    DEPENDS zuloid
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
