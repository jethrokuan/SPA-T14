# Simple Component
set(srcs ${srcs}
  src/simple_source/parser/TestAssign.cpp
  src/simple_source/parser/TestCondExpr.cpp
  src/simple_source/parser/TestExpr.cpp
  src/simple_source/parser/TestIf.cpp
  src/simple_source/parser/TestPrint.cpp
  src/simple_source/parser/TestProcedure.cpp
  src/simple_source/parser/TestRead.cpp
  src/simple_source/parser/TestStatementList.cpp
  src/simple_source/parser/TestWhile.cpp
  src/simple_source/parser/TestInterface.cpp
  src/simple_source/TestSimpleLexer.cpp
  )

set(srcs ${srcs}
  src/main.cpp
  src/TestPKB.cpp
  src/TestQueryBuilder.cpp
  src/TestQueryConstraints.cpp
  src/TestQueryPreProcessor.cpp
  src/TestQueryTokenizer.cpp
  src/TestQueryValidator.cpp
  src/TestQuoteIdent.cpp
  src/TestSuchThat.cpp
  src/TestSynonym.cpp
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)

