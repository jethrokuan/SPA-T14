#!/bin/bash
# Run Autotester on System Tests


# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output

# RUN BASIC TESTS (ITERATION TWO SPECIFIC QUERIES)
build/src/autotester/autotester tests/system_tests/iteration-2/1-300-chain-of-calls.txt tests/system_tests/iteration-2/1-300-chain-of-calls-query.txt tests/system_tests/output/1.xml

build/src/autotester/autotester tests/system_tests/iteration-2/2-basic-call.txt tests/system_tests/iteration-2/2-basic-call-query.txt tests/system_tests/output/2-call.xml
build/src/autotester/autotester tests/system_tests/iteration-2/2-basic-call.txt tests/system_tests/iteration-2/2-basic-call-t-query.txt tests/system_tests/output/2-callt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/3-basic-call-t.txt tests/system_tests/iteration-2/3-basic-call-t-query.txt tests/system_tests/output/3-callt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/4-nest-while-next.txt tests/system_tests/iteration-2/4-nest-while-next-queries.txt tests/system_tests/output/4-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/4-nest-while-next.txt tests/system_tests/iteration-2/4-nest-while-next-t-queries.txt tests/system_tests/output/4-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/5-nest-if-next.txt tests/system_tests/iteration-2/5-nest-if-next-queries.txt tests/system_tests/output/5-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/5-nest-if-next.txt tests/system_tests/iteration-2/5-nest-if-next-t-queries.txt tests/system_tests/output/5-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/6-nest-if-balanced-next-queries.txt tests/system_tests/iteration-2/6-nest-if-balanced-next-queries.txt tests/system_tests/output/6-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/6-nest-if-balanced-next-queries.txt tests/system_tests/iteration-2/6-nest-if-balanced-next-t-queries.txt tests/system_tests/output/6-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/7-mixed-nest-next.txt tests/system_tests/iteration-2/7-mixed-nest-next-queries.txt tests/system_tests/output/7-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/7-mixed-nest-next.txt tests/system_tests/iteration-2/7-mixed-nest-next-t-queries.txt tests/system_tests/output/7-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/8-basic-uses-modifies.txt tests/system_tests/iteration-2/8-basic-uses-modifies-queries.txt tests/system_tests/output/8.xml

build/src/autotester/autotester tests/system_tests/iteration-2/9-basic-with-clause.txt tests/system_tests/iteration-2/9-basic-with-clause-queries.txt tests/system_tests/output/9.xml
build/src/autotester/autotester tests/system_tests/iteration-2/10-basic-pattern.txt tests/system_tests/iteration-2/10-basic-pattern-queries.txt tests/system_tests/output/10.xml
