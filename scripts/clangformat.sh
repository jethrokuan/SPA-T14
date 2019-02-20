#!/usr/bin/env bash
find src \
     \( -name '*.c' \
     -o -name '*.cc' \
     -o -name '*.cpp' \
     -o -name '*.h' \
     -o -name '*.hh' \
     -o -name '*.hpp' \) \
     -exec clang-format -style=file -i '{}' \;

if [[ -z $(git status -s) ]]
then
    exit 0
else
    exit 1
fi
