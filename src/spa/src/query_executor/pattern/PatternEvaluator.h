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
  const QE::Query* query;
  PKBManager* pkb;
  QueryConstraints& qc;

  bool handlePatternLHSUnderscore(const Synonym& syn,
                                  const QE::ExpressionSpec& pattern_rhs);
  bool handlePatternLHSQuoteIdent(const Synonym& syn, const std::string lhs,
                                  const QE::ExpressionSpec& pattern_rhs);
  bool handlePatternLHSSynonym(const Synonym& syn, const Synonym& lhs,
                               const QE::ExpressionSpec& pattern_rhs);

 public:
  PatternEvaluator(const Query* query, PKBManager* pkb, QueryConstraints& qc)
      : query(query), pkb(pkb), qc(qc){};
  bool evaluate();
};