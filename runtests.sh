#!/usr/bin/env bash
# Run Autotester on System Tests


# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output
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
# 1 Synonym, 1 Clause (patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1011-p.txt tests/system_tests/output/1011-p.xml
# 2 Synonym, 1 Clause (patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1021-p.txt tests/system_tests/output/1021-p.xml
# 1 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1012.txt tests/system_tests/output/1012.xml
# 2 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1022.txt tests/system_tests/output/1022.xml

# Temp tests with patterns
# 1 Synonym, 1 Clause (patterns)

build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1011-p.txt tests/system_tests/output/1011-p.xml
# 2 Synonym, 1 Clause (patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1021-p.txt tests/system_tests/output/1021-p.xml
# 1 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1012.txt tests/system_tests/output/1012.xml
# 2 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1022.txt tests/system_tests/output/1022.xml
# 2 Synonym, 2 Clause (su1ch that and patterns), With increased complexity - test associative patterns 
build/src/autotester/autotester tests/system_tests/src/1.txt tests/system_tests/query/1/patterns/1022-new.txt tests/system_tests/output/1022-increasedComplexity.xml

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

# TEMP Tests with patterns
# 1 Synonym, 1 Clause (patterns)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/patterns/2011-p.txt tests/system_tests/output/2011-p.xml
# 2 Synonym, 1 Clause (patterns)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/patterns/2021-p.txt tests/system_tests/output/2021-p.xml
# 1 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/patterns/2012.txt tests/system_tests/output/2012.xml
# 2 Synonym, 2 Clause (such that and patterns)
build/src/autotester/autotester tests/system_tests/src/2.txt tests/system_tests/query/2/patterns/2022.txt tests/system_tests/output/2022.xml


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
# 2 Synonym, 2 Clause (such that and patterns), With increased complexity - test associative patterns 
build/src/autotester/autotester tests/system_tests/src/4.txt tests/system_tests/query/4/patterns/4022-new.txt tests/system_tests/output/4022-increasedComplexity.xml


# RUN TESTS FOR SIMPLE SOURCE 5: MULTIPLE PROCEDURES // NOT REQUIRED FOR ITERATION 1

# Tests without patterns
# 1 Synonym, 0 Clause
build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5010.txt tests/system_tests/output/5010.xml
# 1 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5011-st.txt tests/system_tests/output/5011-st.xml
# 2 Synonym, 1 Clause (such that)
build/src/autotester/autotester tests/system_tests/src/5.txt tests/system_tests/query/5/5021-st.txt tests/system_tests/output/5021-st.xml


# RUN BASIC TESTS (ITERATION TWO SPECIFIC QUERIES)
build/src/autotester/autotester tests/system_tests/iteration-2/1-300-chain-of-calls.txt tests/system_tests/iteration-2/1-300-chain-of-calls-query.txt tests/system_tests/output/1.xml

build/src/autotester/autotester tests/system_tests/iteration-2/2-basic-call.txt tests/system_tests/iteration-2/2-basic-call-query.txt tests/system_tests/output/2-call.xml
build/src/autotester/autotester tests/system_tests/iteration-2/2-basic-call.txt tests/system_tests/iteration-2/2-basic-call-t-query.txt tests/system_tests/output/2-callt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/3-basic-call-t.txt tests/system_tests/iteration-2/3-basic-call-t-query.txt tests/system_tests/output/3-callt.xml

build/src/autotester/autotester tests/system_tests/iteration-2/4-nest-while-next.txt tests/system_tests/iteration-2/4-nest-while-next-queries.txt tests/system_tests/output/4-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/4-nest-while-next.txt tests/system_tests/iteration-2/4-nest-while-next-t-queries.txt tests/system_tests/output/4-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/5-nest-if-next.txt tests/system_tests/iteration-2/5-nest-if-next-queries.txt tests/system_tests/output/5-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/5-nest-if-next.txt tests/system_tests/iteration-2/5-nest-if-next-t-queries.txt tests/system_tests/output/5-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/6-nest-if-next-balanced.txt tests/system_tests/iteration-2/6-nest-if-balanced-next-queries.txt tests/system_tests/output/6-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/6-nest-if-next-balanced.txt tests/system_tests/iteration-2/6-nest-if-balanced-next-t-queries.txt tests/system_tests/output/6-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/7-mixed-nest-next.txt tests/system_tests/iteration-2/7-mixed-nest-next-queries.txt tests/system_tests/output/7-next.xml
build/src/autotester/autotester tests/system_tests/iteration-2/7-mixed-nest-next.txt tests/system_tests/iteration-2/7-mixed-nest-next-t-queries.txt tests/system_tests/output/7-next-t.xml

build/src/autotester/autotester tests/system_tests/iteration-2/8-basic-uses-modifies.txt tests/system_tests/iteration-2/8-basic-uses-modifies-queries.txt tests/system_tests/output/8.xml

build/src/autotester/autotester tests/system_tests/iteration-2/10-basic-pattern.txt tests/system_tests/iteration-2/10-basic-pattern-queries.txt tests/system_tests/output/10.xml

# RUN INVALID TESTS (ITERATION TWO)
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-calls-query.txt tests/system_tests/output/1-invalid-calls-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-calls-t-query.txt tests/system_tests/output/1-invalid-calls-t-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-next-query.txt tests/system_tests/output/1-invalid-next-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/1-invalid-source.txt tests/system_tests/iteration-2/invalids/1-invalid-next-t-query.txt tests/system_tests/output/1-invalid-next-t-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/2-invalid-calls-source.txt tests/system_tests/iteration-2/invalids/2-invalid-calls-query.txt tests/system_tests/output/2-invalid-calls-query.txt.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/3-invalid-calls-t-source.txt tests/system_tests/iteration-2/invalids/3-invalid-calls-t-query.txt tests/system_tests/output/3-invalid-calls-t-query.txt.xml
build/src/autotester/autotester tests/system_tests/iteration-2/invalids/4-invalid-uses-source.txt tests/system_tests/iteration-2/invalids/4-invalid-uses-query.txt tests/system_tests/output/4-invalid-uses-query.txt.xml