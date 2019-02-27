#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class PatternEvaluator {
 private:
  QE::Query* query;
  PKBManager* pkb;

  AllowedValuesPairOrBool handlePatternLHSUnderscore(
      Synonym& syn, QE::ExpressionSpec& pattern_rhs);
  AllowedValuesPairOrBool handlePatternLHSQuoteIdent(
      Synonym& syn, std::string lhs, QE::ExpressionSpec& pattern_rhs);
  AllowedValuesPairOrBool handlePatternLHSSynonym(
      Synonym& syn, Synonym& lhs, QE::ExpressionSpec& pattern_rhs);

 public:
  PatternEvaluator(Query* query, PKBManager* pkb) : query(query), pkb(pkb){};
  AllowedValuesPairOrBool evaluate();
};