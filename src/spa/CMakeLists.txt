# PQL Query Evaluator files
set(srcs ${srcs}
  src/query_evaluator/query_preprocessor.cpp
  src/query_evaluator/query_evaluator.cpp
  src/query_evaluator/query_tokenizer.cpp
  src/query_evaluator/pql.cpp
  )
set(headers ${headers}
  src/query_evaluator/query_preprocessor.h
  src/query_evaluator/query_tokenizer.h
  src/query_evaluator/query_evaluator.h
  src/query_evaluator/pql.h
 )
  
# SIMPLE parser files
set(srcs ${srcs}
  src/simple_parser/lexer.cpp
  src/simple_parser/parser.cpp
  )
set(headers ${headers}
  src/simple_parser/lexer.h
  src/simple_parser/token.h
  src/simple_parser/parser.h
  )
  
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
