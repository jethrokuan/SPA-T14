#!/usr/bin/env bash
# Run Autotester on System Tests


# CREATE THE OUTPUT DIRECTORY
mkdir -p tests/system_tests/output

# ITERATION 1
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1010.txt tests/system_tests/output/iteration1-1010.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1011-p.txt tests/system_tests/output/iteration1-1011-p.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1011-st.txt tests/system_tests/output/iteration1-1011-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1012.txt tests/system_tests/output/iteration1-1012.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1020.txt tests/system_tests/output/iteration1-1020.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1021-p.txt tests/system_tests/output/iteration1-1021-p.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1021-st.txt tests/system_tests/output/iteration1-1021-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1022.txt tests/system_tests/output/iteration1-1022.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/1.txt T14/tests/system_tests/iteration-1/1022-new.txt tests/system_tests/output/iteration1-1022-new.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2010.txt tests/system_tests/output/iteration1-2010.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2011-p.txt tests/system_tests/output/iteration1-2011-p.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2011-st.txt tests/system_tests/output/iteration1-2011-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2012.txt tests/system_tests/output/iteration1-2012.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2020.txt tests/system_tests/output/iteration1-2020.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2021-p.txt tests/system_tests/output/iteration1-2021-p.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2021-st.txt tests/system_tests/output/iteration1-2021-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/2.txt T14/tests/system_tests/iteration-1/2022.txt tests/system_tests/output/iteration1-2022.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/4.txt T14/tests/system_tests/iteration-1/4010.txt tests/system_tests/output/iteration1-4010.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/4.txt T14/tests/system_tests/iteration-1/4011-st.txt tests/system_tests/output/iteration1-4011-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/4.txt T14/tests/system_tests/iteration-1/4021-st.txt tests/system_tests/output/iteration1-4021-st.xml
build/src/autotester/autotester T14/tests/system_tests/iteration-1/4.txt T14/tests/system_tests/iteration-1/4022.txt tests/system_tests/output/iteration1-4022.xml

# ITERATION 2 - SET 0 
build/src/autotester/autotester tests/system_tests/iteration-2/set0/1-300-chain-of-calls.txt tests/system_tests/iteration-2/set0/1-300-chain-of-calls-query.txt tests/system_tests/output/iteration2-1-300-chain-of-calls-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/2-basic-call.txt tests/system_tests/iteration-2/set0/2-basic-call-query.txt tests/system_tests/output/iteration2-2-basic-call-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/2-basic-call.txt tests/system_tests/iteration-2/set0/2-basic-call-t-query.txt tests/system_tests/output/iteration2-2-basic-call-t-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/3-basic-call-t.txt tests/system_tests/iteration-2/set0/3-basic-call-t-query.txt tests/system_tests/output/iteration2-3-basic-call-t-query.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/4-nest-while-next.txt tests/system_tests/iteration-2/set0/4-nest-while-next-queries.txt tests/system_tests/output/iteration2-4-nest-while-next-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/4-nest-while-next.txt tests/system_tests/iteration-2/set0/4-nest-while-next-t-queries.txt tests/system_tests/output/iteration2-4-nest-while-next-t-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/5-nest-if-next.txt tests/system_tests/iteration-2/set0/5-nest-if-next-queries.txt tests/system_tests/output/iteration2-5-nest-if-next-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/5-nest-if-next.txt tests/system_tests/iteration-2/set0/5-nest-if-next-t-queries.txt tests/system_tests/output/iteration2-5-nest-if-next-t-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/6-nest-if-next-balanced.txt tests/system_tests/iteration-2/set0/6-nest-if-balanced-next-queries.txt tests/system_tests/output/iteration2-6-nest-if-balanced-next-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/6-nest-if-next-balanced.txt tests/system_tests/iteration-2/set0/6-nest-if-balanced-next-t-queries.txt tests/system_tests/output/iteration2-6-nest-if-balanced-next-t-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/7-mixed-nest-next.txt tests/system_tests/iteration-2/set0/7-mixed-nest-next-queries.txt tests/system_tests/output/iteration2-7-mixed-nest-next-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/7-mixed-nest-next.txt tests/system_tests/iteration-2/set0/7-mixed-nest-next-t-queries.txt tests/system_tests/output/iteration2-7-mixed-nest-next-t-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/8-basic-uses-modifies.txt tests/system_tests/iteration-2/set0/8-basic-uses-modifies-queries.txt tests/system_tests/output/iteration2-8-basic-uses-modifies-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/9-basic-with-clause.txt tests/system_tests/iteration-2/set0/9-basic-with-clause-queries.txt tests/system_tests/output/iteration2-9-basic-with-clause-queries.xml
build/src/autotester/autotester tests/system_tests/iteration-2/set0/10-basic-pattern.txt tests/system_tests/iteration-2/set0/10-basic-pattern-queries.txt tests/system_tests/output/iteration2-10-basic-pattern-queries.xml


# ITERATION 2 - SET 1
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-calls.txt tests/system_tests/output/iteration2-1-calls.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-callsT.txt tests/system_tests/output/iteration2-1-callsT.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-follows.txt tests/system_tests/output/iteration2-1-follows.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-followsT.txt tests/system_tests/output/iteration2-1-followsT.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-modifies.txt tests/system_tests/output/iteration2-1-modifies.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-multi.txt tests/system_tests/output/iteration2-1-multi.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-next.txt tests/system_tests/output/iteration2-1-next.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-nextT.txt tests/system_tests/output/iteration2-1-nextT.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-parent.txt tests/system_tests/output/iteration2-1-parent.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-parentT.txt tests/system_tests/output/iteration2-1-parentT.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-pattern-assign.txt tests/system_tests/output/iteration2-1-pattern-assign.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-pattern-container.txt tests/system_tests/output/iteration2-1-pattern-container.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-uses.txt tests/system_tests/output/iteration2-1-uses.xml
tests/system_tests/iteration-2/set1/source-1.txt tests/system_tests/iteration-2/set1/1-collated-basic-test.txt tests/system_tests/output/iteration2-1-collated-basic-test.xml
