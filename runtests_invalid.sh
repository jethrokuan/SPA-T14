#!/usr/bin/env bash
# Run Autotester on System Tests


# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output
# RUN TESTS FOR SIMPLE SOURCE 1: BASIC SOURCE

# Test invalids
# 1 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-calls-query.txt tests/system_tests/output/1-invalid-calls-query.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-calls-t-query.txt tests/system_tests/output/1-invalid-calls-t-query.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-next-query.txt tests/system_tests/output/1-invalid-next-query.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-next-t-query.txt tests/system_tests/output/1-invalid-next-t-query.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/2-invalid-calls-source.txt tests/system_tests/iteration-2/invalids/2-invalid-calls-query.txt tests/system_tests/output/2-invalid-calls-query.txt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/3-invalid-calls-t-source.txt tests/system_tests/iteration-2/invalids/3-invalid-calls-t-query.txt tests/system_tests/output/3-invalid-calls-t-query.txt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/invalids/4-invalid-uses-source.txt tests/system_tests/iteration-2/invalids/4-invalid-uses-query.txt tests/system_tests/output/4-invalid-uses-query.txt.xml
