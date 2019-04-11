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

# Query Executor files
set(srcs ${srcs}
  src/query_executor/query_executor.cpp
  src/query_executor/query_executor_utils.cpp
  src/query_executor/suchthat/SuchThatEvaluator.cpp
  src/query_executor/pattern/PatternEvaluator.cpp
  src/query_executor/constraint_solver/constraint_database.cpp
  src/query_executor/constraint_solver/constraint_table.cpp
  src/query_executor/clause_prioritizer.cpp
  src/query_executor/clause_functions.cpp
  src/query_executor/with/WithEvaluator.cpp
  )
set(headers ${headers}
  src/query_executor/query_executor.h
  src/query_executor/constraint_solver/constraint_database.h
  src/query_executor/constraint_solver/constraint_table.h
  src/query_executor/clause_prioritizer.h
  src/query_executor/suchthat/SuchThatEvaluator.h
  src/query_executor/suchthat/FollowsTEvaluator.h
  src/query_executor/suchthat/ParentTEvaluator.h
  src/query_executor/suchthat/ModifiesSEvaluator.h
  src/query_executor/suchthat/UsesSEvaluator.h
  src/query_executor/suchthat/FollowsEvaluator.h
  src/query_executor/suchthat/ParentEvaluator.h
  src/query_executor/pattern/PatternEvaluator.h
  src/query_executor/pattern/AssignPatternEvaluator.h
  src/query_executor/suchthat/NextEvaluator.h
  src/query_executor/suchthat/NextTEvaluator.h
  src/query_executor/suchthat/CallsEvaluator.h
  src/query_executor/suchthat/CallsTEvaluator.h
  src/query_executor/suchthat/ModifiesPEvaluator.h
  src/query_executor/suchthat/UsesPEvaluator.h
  src/query_executor/suchthat/AffectsEvaluator.h
  src/query_executor/suchthat/AffectsTEvaluator.h
  src/query_executor/with/WithEvaluator.h
  )

# PQL Query Builder files
set(srcs ${srcs}
  src/query_builder/query_builder.cpp
  src/query_builder/core/query_lexer.cpp
  src/query_builder/core/query_parser.cpp
  src/query_builder/core/query_validator.cpp
  src/query_builder/pql/design_entity.cpp
  src/query_builder/pql/quoteident.cpp
  src/query_builder/pql/query.cpp
  src/query_builder/pql/relation.cpp
  src/query_builder/pql/synonym.cpp
  src/query_builder/pql/attrref.cpp
  src/query_builder/pql/matcher.cpp
  src/query_builder/pql/with_cond.cpp
  )

set(headers ${headers}
  src/query_builder/query_builder.h
  src/query_builder/core/query_lexer.h
  src/query_builder/core/query_parser.h
  src/query_builder/core/query_validator.h
  src/query_builder/pql/pql.h
  src/query_builder/pql/declaration.h
  src/query_builder/pql/design_entity.h
  src/query_builder/pql/query.h
  src/query_builder/pql/quoteident.h
  src/query_builder/pql/rel_cond.h
  src/query_builder/pql/with_cond.h
  src/query_builder/pql/matcher.h
  src/query_builder/pql/pattern_cond.h
  src/query_builder/pql/relation.h
  src/query_builder/pql/result.h
  src/query_builder/pql/ref.h
  src/query_builder/pql/attrref.h
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
