cmake_minimum_required (VERSION 3.4.1)
project (Z64C)

file (GLOB Z64C_SRC "*.h" "*.c")
add_executable (Z64C ${Z64C_SRC})
