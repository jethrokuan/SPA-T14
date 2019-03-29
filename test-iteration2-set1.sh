#!/bin/bash
# Run Autotester on System Tests

# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output
# RUN TESTS FOR SIMPLE SOURCE 1: BASIC SOURCE

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-calls.txt TEMPTEST/OUTPUT/1-calls.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-callsT.txt TEMPTEST/OUTPUT/1-callsT.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-follows.txt TEMPTEST/OUTPUT/1-follows.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-followsT.txt TEMPTEST/OUTPUT/1-followsT.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-modifies.txt TEMPTEST/OUTPUT/1-modifies.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-next.txt TEMPTEST/OUTPUT/1-next.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-nextT.txt TEMPTEST/OUTPUT/1-nextT.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-parent.txt TEMPTEST/OUTPUT/1-parent.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-parentT.txt TEMPTEST/OUTPUT/1-parentT.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-pattern-assign.txt TEMPTEST/OUTPUT/1-pattern-assign.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-pattern-container.txt TEMPTEST/OUTPUT/1-pattern-container.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-uses.txt TEMPTEST/OUTPUT/1-uses.xml

# build/src/autotester/autotester TEMPTEST/set1/source-1.txt TEMPTEST/set1/1-multi.txt TEMPTEST/OUTPUT/1-multi.xml

build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt TEMPTEST/set1/1-collated-basic-test.txt tests/system_tests/output/1-collated-basic-test.xml

build/src/autotester/autotester tests/system_tests/iteration-2/set1/source-1.txt TEMPTEST/set1/1-multi.txt tests/system_tests/output/1-multi.xml
