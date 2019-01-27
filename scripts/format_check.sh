#!/usr/bin/env bash
FILES=$(find src/ -iname *.h -o -iname *.cpp)

for FILE in $FILES; do
    clang-format $FILE | cmp $FILE > /dev/null

    if [ $? -ne 0 ]; then
        echo "INCORRECT FORMATTING: $FILE" >&2
        RETURN=1
    fi

    exit $RETURN
done

exit 0
