#!/usr/bin/env bash

# FIXME - Amalgamation is still buggy and weird.

find src/ -name '*.c' -exec cat {} + > zuloid.c
gcc -std=c99 -I include/ -I lib/ -I lib/plibsys/src/ -I build/ -Lbuild/ -l:libplibsys.so zuloid.c -o zuloid

#cat include/**/*.h | grep -v "^#include \"" > zuloid.c

#echo -i include/ $() amalgamate.c
