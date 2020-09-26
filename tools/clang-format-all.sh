#!/usr/bin/env bash

find include src test -name "*.h" -o -name "*.c" | xargs clang-format -i
