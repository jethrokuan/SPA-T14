#!/bin/bash
# Run Autotester on System Tests

# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output
# RUN TESTS FOR SIMPLE SOURCE 1: BASIC SOURCE

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-calls.txt tests/system_tests/output/1-calls.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-callsT.txt tests/system_tests/output/1-callsT.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-follows.txt tests/system_tests/output/1-follows.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-followsT.txt tests/system_tests/output/1-followsT.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-modifies.txt tests/system_tests/output/1-modifies.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-next.txt tests/system_tests/output/1-next.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-nextT.txt tests/system_tests/output/1-nextT.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-parent.txt tests/system_tests/output/1-parent.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-parentT.txt tests/system_tests/output/1-parentT.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-pattern-assign.txt tests/system_tests/output/1-pattern-assign.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-pattern-container.txt tests/system_tests/output/1-pattern-container.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-uses.txt tests/system_tests/output/1-uses.xml

# build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-multi.txt tests/system_tests/output/1-multi.xml

build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-collated-basic-test.txt tests/system_tests/output/1-collated-basic-test.xml


 # build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-collated-basic-test.txt.txt TEMPTEST/OUTPUT/1-collated.xml
