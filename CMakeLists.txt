cmake_minimum_required (VERSION 3.4.1)
project (Z64C)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 14)

set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/")

set (HIREDIS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lib/hiredis/libhiredis.a")
add_custom_command (
	OUTPUT ${HIREDIS_SRC}
	COMMAND make
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/hiredis/" )
add_library (HIREDIS SHARED ${HIREDIS_SRC})
set_target_properties (HIREDIS PROPERTIES LINKER_LANGUAGE CXX)

set (CATCH2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/Catch2/single_include/")
set (CATCH2_SRC "${CATCH2_DIR}/catch.hpp")
add_library (CATCH2 INTERFACE)
target_sources (CATCH2 INTERFACE ${CATCH2_SRC})

file (GLOB Z64C_SRC "src/*.h" "src/*.c")
add_library (Z64C_LIB SHARED ${Z64C_SRC})
target_include_directories (Z64C_LIB PUBLIC ${INCLUDE_DIR})
target_link_libraries (Z64C_LIB HIREDIS)

add_executable (Z64C ${Z64C_SRC})
target_include_directories (Z64C PUBLIC ${INCLUDE_DIR})
target_link_libraries (Z64C Z64C_LIB)

file (GLOB Z64C_TEST_SRC "test/*.cpp")
add_executable (Z64C-test ${Z64C_TEST_SRC})
target_include_directories (Z64C-test PUBLIC "src/" ${INCLUDE_DIR} ${CATCH2_DIR})
target_link_libraries (Z64C-test Z64C_LIB)
enable_testing ()
add_test (NAME Z64C COMMAND Z64C-test)
