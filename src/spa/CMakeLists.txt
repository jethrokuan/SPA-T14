# PKB files
set(srcs ${srcs}
  src/program_knowledge_base/pkb_manager.cpp
  src/program_knowledge_base/pkb_preprocessor.cpp
  src/program_knowledge_base/pkb_storage.cpp
  )
set(headers ${headers}
  src/program_knowledge_base/pkb_manager.h
  src/program_knowledge_base/pkb_preprocessor.h
  src/program_knowledge_base/pkb_storage.h
  )

# PKB files
set(srcs ${srcs}
  src/utils/utils.cpp
  )
set(headers ${headers}
  src/utils/utils.h
  )

# SIMPLE parser files
set(srcs ${srcs}
  src/structs/node.cpp
  )
set(headers ${headers}
  src/structs/node.h
  )

# SPA Manager files
set(srcs ${srcs}
  src/spa_manager/spa_manager.cpp
  )
set(headers ${headers}
  src/spa_manager/spa_manager.h
  )

# Query Manager files
set(srcs ${srcs}
  src/query_manager/query_manager.cpp
  src/query_manager/query_manager_utils.cpp
  src/query_manager/constraint_solver.cpp
  src/query_manager/relations/SuchThatEvaluator.cpp
  src/query_manager/pattern/PatternEvaluator.cpp
  )
set(headers ${headers}
  src/query_manager/query_manager.h
  src/query_manager/constraint_solver.h
  src/query_manager/relations/SuchThatEvaluator.h
  src/query_manager/relations/FollowsTEvaluator.h
  src/query_manager/relations/ParentTEvaluator.h
  src/query_manager/relations/ModifiesSEvaluator.h
  src/query_manager/relations/UsesSEvaluator.h
  src/query_manager/relations/FollowsEvaluator.h
  src/query_manager/relations/ParentEvaluator.h
  src/query_manager/pattern/PatternEvaluator.h
  )

# PQL Query Evaluator files
set(srcs ${srcs}
  src/query_evaluator/query_evaluator.cpp
  src/query_evaluator/core/query_preprocessor.cpp
  src/query_evaluator/core/query_tokenizer.cpp
  src/query_evaluator/core/query_validator.cpp
  src/query_evaluator/pql/design_entity.cpp
  src/query_evaluator/pql/factor.cpp
  src/query_evaluator/pql/pattern.cpp
  src/query_evaluator/pql/quoteident.cpp
  src/query_evaluator/pql/query.cpp
  src/query_evaluator/pql/relation.cpp
  src/query_evaluator/pql/suchthat.cpp
  src/query_evaluator/pql/synonym.cpp
 )
set(headers ${headers}
  src/query_evaluator/query_evaluator.h
  src/query_evaluator/core/query_preprocessor.h
  src/query_evaluator/core/query_tokenizer.h
  src/query_evaluator/core/query_validator.h
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
 )
  
# SIMPLE parser files
set(srcs ${srcs}
  src/simple_parser/interface.cpp
  src/simple_parser/lexer.cpp
  src/simple_parser/parser.cpp
  src/simple_parser/pratt.cpp
  src/simple_parser/token.cpp
  )
set(headers ${headers}
  src/simple_parser/exceptions.h
  src/simple_parser/interface.h
  src/simple_parser/lexer.h
  src/simple_parser/parser.h
  src/simple_parser/token.h
  src/simple_parser/pratt.h
  )
  
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
