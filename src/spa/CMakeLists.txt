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
  src/query_executor/query_executor.cpp
  src/query_executor/query_executor_utils.cpp
  src/query_executor/constraint_solver/constraint_solver.h
  src/query_executor/suchthat/SuchThatEvaluator.cpp
  src/query_executor/pattern/PatternEvaluator.cpp
  )
set(headers ${headers}
  src/query_executor/query_executor.h
  src/query_executor/constraint_solver/constraint_solver.cpp
  src/query_executor/constraint_solver/constraint_solver_utils.cpp
  src/query_executor/suchthat/SuchThatEvaluator.h
  src/query_executor/suchthat/FollowsTEvaluator.h
  src/query_executor/suchthat/ParentTEvaluator.h
  src/query_executor/suchthat/ModifiesSEvaluator.h
  src/query_executor/suchthat/UsesSEvaluator.h
  src/query_executor/suchthat/FollowsEvaluator.h
  src/query_executor/suchthat/ParentEvaluator.h
  src/query_executor/pattern/PatternEvaluator.h
  )

# PQL Query Builder files
set(srcs ${srcs}
  src/query_builder/query_builder.cpp
  src/query_builder/core/query_preprocessor.cpp
  src/query_builder/core/query_tokenizer.cpp
  src/query_builder/core/query_validator.cpp
  src/query_builder/pql/design_entity.cpp
  src/query_builder/pql/factor.cpp
  src/query_builder/pql/pattern.cpp
  src/query_builder/pql/quoteident.cpp
  src/query_builder/pql/query.cpp
  src/query_builder/pql/relation.cpp
  src/query_builder/pql/suchthat.cpp
  src/query_builder/pql/synonym.cpp
 )
set(headers ${headers}
  src/query_builder/query_builder.h
  src/query_builder/core/query_preprocessor.h
  src/query_builder/core/query_tokenizer.h
  src/query_builder/core/query_validator.h
  src/query_builder/pql/pql.h
  src/query_builder/pql/declaration.h
  src/query_builder/pql/design_entity.h
  src/query_builder/pql/factor.h
  src/query_builder/pql/pattern.h
  src/query_builder/pql/query.h
  src/query_builder/pql/quoteident.h
  src/query_builder/pql/relation.h
  src/query_builder/pql/stmtentref.h
  src/query_builder/pql/suchthat.h
  src/query_builder/pql/synonym.h
  src/query_builder/pql/underscore.h
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
