#!/usr/bin/env bash

# Run Autotester on System Tests

# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output

build/src/autotester/autotester tests/affects_source.txt tests/affects_queries.txt tests/system_tests/output/alex_affects.xml
build/src/autotester/autotester tests/Combination_source.txt tests/Combination_queries.txt tests/system_tests/output/alex_Combination.xml
build/src/autotester/autotester tests/Merging2_source.txt tests/Merging2_queries.txt tests/system_tests/output/alex_Merging2.xml
build/src/autotester/autotester tests/Multiple_nested_adv_source.txt tests/Multiple_nested_adv_queries.txt tests/system_tests/output/alex_multiple.xml
