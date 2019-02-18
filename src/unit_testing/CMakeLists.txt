set(srcs
  src/main.cpp
  src/TestPKB.cpp
  src/TestQueryEvaluator.cpp
  src/TestQueryManager.cpp
  src/TestQueryPreProcessor.cpp
  src/TestQueryTokenizer.cpp
  src/TestQueryValidator.cpp
  src/TestQuoteIdent.cpp
  src/simple_source/parser/TestProcedure.cpp
  src/simple_source/parser/TestStatementList.cpp
  src/simple_source/TestSimpleLexer.cpp
  src/simple_source/TestExprParser.cpp
  src/simple_source/TestSimpleParser.cpp
  src/TestSuchThat.cpp
  src/TestSynonym.cpp
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs})

target_link_libraries(unit_testing spa)

