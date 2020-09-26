#!/bin/sh

# https://stackoverflow.com/questions/28896909/
find include src test -iname "*.h" -o -iname "*.c" | xargs clang-format -i;

exit 0;
