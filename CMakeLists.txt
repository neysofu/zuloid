cmake_minimum_required (VERSION 3.4.1)
project (Z64C)

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 14)

set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")

find_package (ArrayFire REQUIRED)

# xxHash
set (XXHASH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/xxHash/")
set (XXHASH_SRC "${XXHASH_DIR}/xxhash.c")
add_library (XXHASH INTERFACE)
target_sources (XXHASH INTERFACE ${XXHASH_SRC})

# Catch2
set (CATCH2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/Catch2/single_include/")
set (CATCH2_SRC "${CATCH2_DIR}/catch.hpp")
add_library (CATCH2 INTERFACE)
target_sources (CATCH2 INTERFACE ${CATCH2_SRC})

# MT19937-64
add_library (MT19937_64 "${CMAKE_CURRENT_SOURCE_DIR}/lib/mt19937-64/mt19937-64.c")

# Z64C_LIB
set (Z64C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app/engine/include/")
file (GLOB_RECURSE Z64C_SRC "${Z64C_INCLUDE_DIR}*.h" "app/engine/src/*.c")
add_library (Z64C_LIB STATIC ${Z64C_SRC})
target_include_directories (Z64C_LIB PUBLIC ${LIB_DIR})
target_include_directories (Z64C_LIB PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_LIB ArrayFire::afcpu)
target_link_libraries (Z64C_LIB XXHASH)
target_link_libraries (Z64C_LIB THPOOL)
target_link_libraries (Z64C_LIB MT19937_64)

# Z64C
add_executable (Z64C ${Z64C_SRC})
target_link_libraries (Z64C Z64C_LIB)
set_target_properties (Z64C PROPERTIES COMPILE_FLAGS "-save-temps")

# Z64C_TEST
file (GLOB_RECURSE Z64C_TEST_SRC "app/engine/test/*.cpp")
add_executable (Z64C_TEST ${Z64C_TEST_SRC})
target_include_directories (Z64C_TEST PUBLIC ${Z64C_INCLUDE_DIR})
target_link_libraries (Z64C_TEST Z64C_LIB)
target_link_libraries (Z64C_TEST CATCH2)
enable_testing ()
add_test (NAME Z64C COMMAND Z64C_TEST)
