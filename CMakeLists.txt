cmake_minimum_required (VERSION 3.4.1)
project (Z64C)

set (XXHASH_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lib/xxHash/xxhash.o")
add_custom_command (
	OUTPUT ${XXHASH_SRC}
	COMMAND "make"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/xxHash/" )

set (HIREDIS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/lib/hiredis/libhiredis.a")
add_custom_command (
	OUTPUT ${HIREDIS_SRC}
	COMMAND "make"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/hiredis/" )

file (GLOB Z64C_SRC_H "src/*.h")
file (GLOB Z64C_SRC_C "src/*.c")
add_executable (Z64C ${Z64C_SRC_H} ${Z64C_SRC_C} ${XXHASH_SRC} ${HIREDIS_SRC})
target_include_directories (Z64C PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set_target_properties (Z64C PROPERTIES ENABLE_EXPORTS 1)

enable_testing ()
file (GLOB Z64C_TEST_SRC_C "test/*.c")
add_executable (Z64C_TEST ${Z64C_TEST_SRC_C})
target_include_directories (Z64C_TEST PUBLIC "src/")
add_test (Z64C_TEST COMMAND Z64C_TEST)
target_link_libraries (Z64C_TEST Z64C)
