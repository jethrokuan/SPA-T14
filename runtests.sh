#!/bin/bash
# Run Autotester on System Tests

# RUN TESTS FOR SIMPLE SOURCE 1: BASIC SOURCE
# Tests without patterns
# 1 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1010.txt tests/system_tests/output/1010.xml
# 2 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1020.txt tests/system_tests/output/1020.xml
# 1 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1011-st.txt tests/system_tests/output/1011-st.xml
# 2 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1021-st.txt tests/system_tests/output/1021-st.xml

# Tests with patterns
# # 1 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1011-p.txt tests/system_tests/output/1011-p.xml
# # 2 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1021-p.txt tests/system_tests/output/1021-p.xml
# # 1 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1012-p.txt tests/system_tests/output/1012.xml
# # 2 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1022-p.txt tests/system_tests/output/1022.xml


# RUN TESTS FOR SIMPLE SOURCE 2: DEEP NESTING
# Tests without patterns
# 1 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1010.txt tests/system_tests/output/1010.xml
# 2 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1020.txt tests/system_tests/output/1020.xml
# 1 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1011-st.txt tests/system_tests/output/1011-st.xml
# 2 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1021-st.txt tests/system_tests/output/1021-st.xml

# Tests with patterns
# # 1 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1011-p.txt tests/system_tests/output/1011-p.xml
# # 2 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1021-p.txt tests/system_tests/output/1021-p.xml
# # 1 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1012-p.txt tests/system_tests/output/1012.xml
# # 2 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/1022-p.txt tests/system_tests/output/1022.xml
