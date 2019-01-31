# PQL Query Evaluator files
set(srcs ${srcs}
  src/query_evaluator/query_preprocessor.cpp
  src/query_evaluator/query_evaluator.cpp
  src/query_evaluator/query_tokenizer.cpp
  src/query_evaluator/pql/design_entity.cpp
  src/query_evaluator/pql/factor.cpp
  src/query_evaluator/pql/pattern.cpp
  src/query_evaluator/pql/quoteident.cpp
  src/query_evaluator/pql/query.cpp
  src/query_evaluator/pql/relation.cpp
  src/query_evaluator/pql/suchthat.cpp
  src/query_evaluator/pql/synonym.cpp
  src/query_evaluator/pql/util.cpp
 )
set(headers ${headers}
  src/query_evaluator/query_preprocessor.h
  src/query_evaluator/query_tokenizer.h
  src/query_evaluator/query_evaluator.h
  src/query_evaluator/pql/pql.h
  src/query_evaluator/pql/declaration.h
  src/query_evaluator/pql/design_entity.h
  src/query_evaluator/pql/factor.h
  src/query_evaluator/pql/pattern.h
  src/query_evaluator/pql/query.h
  src/query_evaluator/pql/quoteident.h
  src/query_evaluator/pql/relation.h
  src/query_evaluator/pql/stmtentref.h
  src/query_evaluator/pql/suchthat.h
  src/query_evaluator/pql/synonym.h
  src/query_evaluator/pql/underscore.h
  src/query_evaluator/pql/util.h
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
