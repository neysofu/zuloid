#!/usr/bin/env bash

SOURCE_FILES=$(find src include -type f \( -name '*.c' -o -name '*.h' \))

for filename in $SOURCE_FILES; do
    first_line=$(head -n 1 $filename)
    if [[ $first_line != *"SPDX-License-Identifier:"* ]]; then
        echo "$filename does NOT contain SPDX license id."
    fi
done


