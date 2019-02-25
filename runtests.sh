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
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2010.txt tests/system_tests/output/2010.xml
# 2 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2020.txt tests/system_tests/output/2020.xml
# 1 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2011-st.txt tests/system_tests/output/2011-st.xml
# 2 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2021-st.txt tests/system_tests/output/2021-st.xml

# Tests with patterns
# # 1 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2011-p.txt tests/system_tests/output/2011-p.xml
# # 2 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2021-p.txt tests/system_tests/output/2021-p.xml
# # 1 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2012-p.txt tests/system_tests/output/2012.xml
# # 2 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/2022-p.txt tests/system_tests/output/2022.xml

# RUN TESTS FOR SIMPLE SOURCE 3: COMPLEX PATTERNS

# # Tests without patterns
# # 1 Synonym, 0 Clause
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3010.txt tests/system_tests/output/3010.xml
# # 2 Synonym, 0 Clause
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3020.txt tests/system_tests/output/3020.xml
# # 1 Synonym, 1 Clause (such that)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3011-st.txt tests/system_tests/output/3011-st.xml
# # 2 Synonym, 1 Clause (such that)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3021-st.txt tests/system_tests/output/3021-st.xml

# # Tests with patterns
# # 1 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3011-p.txt tests/system_tests/output/3011-p.xml
# # 2 Synonym, 1 Clause (patterns)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3021-p.txt tests/system_tests/output/3021-p.xml
# # 1 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3012-p.txt tests/system_tests/output/3012.xml
# # 2 Synonym, 2 Clause (such that and patterns)
# build/src/autotester/autotester tests/system_tests/src/3.txt tests/system_tests/query/3/3022-p.txt tests/system_tests/output/3022.xml



# OLD TESTS: These tests are the initial system tests created, less exhaustive
# RUN TESTS FOR SIMPLE SOURCE 4: DEEP NESTING

# Tests without patterns
# 1 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/4.txt tests/system_tests/query/4/4010.txt tests/system_tests/output/4010.xml
# 1 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/4.txt tests/system_tests/query/4/4011-st.txt tests/system_tests/output/4011-st.xml
# 2 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/4.txt tests/system_tests/query/4/4021-st.txt tests/system_tests/output/4021-st.xml

# RUN TESTS FOR SIMPLE SOURCE 5: MULTIPLE PROCEDURES // NOT REQUIRED FOR ITERATION 1

# Tests without patterns
# 1 Synonym, 0 Clause
# build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5010.txt tests/system_tests/output/5010.xml
# # 1 Synonym, 1 Clause (such that)
# build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5011-st.txt tests/system_tests/output/5011-st.xml
# # 2 Synonym, 1 Clause (such that)
# build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5021-st.txt tests/system_tests/output/5021-st.xml

