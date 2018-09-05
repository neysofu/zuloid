cmake_minimum_required (VERSION 3.4.1)
project (Z64C)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -ggdb -g")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -frename-registers -funroll-loops -flto")

set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")

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

# Z64C_LIB
file (GLOB Z64C_SRC "app/engine/src/*.h" "app/engine/src/*.c")
add_library (Z64C_LIB STATIC ${Z64C_SRC})
target_include_directories (Z64C_LIB PUBLIC ${INCLUDE_DIR})
target_link_libraries(Z64C_LIB XXHASH)

# Z64C
add_executable (Z64C ${Z64C_SRC})
target_include_directories (Z64C PUBLIC ${INCLUDE_DIR})
target_link_libraries (Z64C Z64C_LIB)

# Z64C_TEST
file (GLOB Z64C_TEST_SRC "app/engine/test/*.cpp")
add_executable (Z64C_TEST ${Z64C_TEST_SRC})
target_include_directories (Z64C_TEST PUBLIC "app/engine/src/" ${INCLUDE_DIR} ${CATCH2_DIR})
target_link_libraries (Z64C_TEST Z64C_LIB)
enable_testing ()
add_test (NAME Z64C COMMAND Z64C_TEST)
