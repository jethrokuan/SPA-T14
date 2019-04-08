#!/usr/bin/env bash
# Run Autotester on System Tests


# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output

build/src/autotester/autotester tests/system_tests/iteration-3/affects/1-source.txt tests/system_tests/iteration-3/affects/1-affects.txt tests/system_tests/output/iter3-1-affects.xml

build/src/autotester/autotester tests/system_tests/iteration-3/affects/2-source.txt tests/system_tests/iteration-3/affects/2-affects.txt tests/system_tests/output/iter3-2-affects.xml
build/src/autotester/autotester tests/system_tests/iteration-3/affects/2-source.txt tests/system_tests/iteration-3/affects/2-affects-t.txt tests/system_tests/output/iter3-2-affects-t.xml

build/src/autotester/autotester tests/system_tests/iteration-3/affects/3-source.txt tests/system_tests/iteration-3/affects/3-affects-both.txt tests/system_tests/output/iter3-3-affects-both.xml

build/src/autotester/autotester tests/system_tests/iteration-3/affects/4-source.txt tests/system_tests/iteration-3/affects/4-affects-double-clause.txt tests/system_tests/output/iter3-4-affects-double-clause.xml
build/src/autotester/autotester tests/system_tests/iteration-3/affects/4-source.txt tests/system_tests/iteration-3/affects/4-affects-t-double-clause.txt tests/system_tests/output/iter3-4-affects-t-double-clause.xml

build/src/autotester/autotester tests/system_tests/iteration-3/affects/5-source.txt tests/system_tests/iteration-3/affects/5-multi-clause.txt tests/system_tests/output/iter3-5-multi.xml