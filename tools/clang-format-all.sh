#!/bin/sh

# https://stackoverflow.com/questions/28896909/
find include src test -name "*.h" -o -name "*.c" | xargs clang-format -i;

exit 0;
