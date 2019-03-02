#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/query_constraints.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class PatternEvaluator {
 private:
  QE::Query* query;
  PKBManager* pkb;
  QueryConstraints& qc;

  bool handlePatternLHSUnderscore(Synonym& syn,
                                  QE::ExpressionSpec& pattern_rhs);
  bool handlePatternLHSQuoteIdent(Synonym& syn, std::string lhs,
                                  QE::ExpressionSpec& pattern_rhs);
  bool handlePatternLHSSynonym(Synonym& syn, Synonym& lhs,
                               QE::ExpressionSpec& pattern_rhs);

 public:
  PatternEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : query(query), pkb(pkb), qc(qc){};
  bool evaluate();
};