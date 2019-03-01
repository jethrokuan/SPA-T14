#include "query_executor/pattern/PatternEvaluator.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/FollowsEvaluator.h"
#include "query_executor/suchthat/FollowsTEvaluator.h"
#include "query_executor/suchthat/ModifiesSEvaluator.h"
#include "query_executor/suchthat/ParentEvaluator.h"
#include "query_executor/suchthat/ParentTEvaluator.h"
#include "query_executor/suchthat/UsesSEvaluator.h"

AllowedValuesPairOrBool PatternEvaluator::evaluate() {
  auto pattern = query->pattern;
  auto pattern_syn = pattern->getSynonym();
  auto pattern_lhs = pattern->getFirstArg();
  auto pattern_rhs = pattern->getSecondArg();
  // pattern-cl : ‘pattern’ syn-assign ‘(‘ entRef ‘,’ expression-spec ’)’
  // entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’
  // expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’ [BASIC-SPA]
  // expression-spec: ‘_’ ‘"’ factor ‘"’ ‘_’ | ‘_’ [ITER1]
  // factor: var_name | const_value [ITER1]

  // Clearly: 6 possible cases (cross product of entRef and expression-spec)
  // Match on entRef
  if (std::get_if<Underscore>(&pattern_lhs)) {
    return handlePatternLHSUnderscore(pattern_syn, pattern_rhs);
  } else if (std::get_if<QuoteIdent>(&pattern_lhs)) {
    return handlePatternLHSQuoteIdent(
        pattern_syn, QueryExecutor::entRefToString(pattern_lhs), pattern_rhs);
  } else if (auto lhs_syn = std::get_if<Synonym>(&pattern_lhs)) {
    return handlePatternLHSSynonym(pattern_syn, *lhs_syn, pattern_rhs);
  } else {
    assert(false);
  }
}

AllowedValuesPairOrBool PatternEvaluator::handlePatternLHSUnderscore(
    Synonym& syn, ExpressionSpec& pattern_rhs) {
  // pattern a (_, <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (_, _) --> all assignments
    auto all_assigns = QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
    return ConstraintSolver::makeAllowedValues(syn, all_assigns);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (_, _"x + y"_) --> partial match, no var constraint
    std::ostringstream stream;
    stream << *duf;
    auto matching_assigns = pkb->getPartialMatchLines(stream.str())
                                .value_or(std::vector<std::string>());

    return ConstraintSolver::makeAllowedValues(syn, matching_assigns);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

AllowedValuesPairOrBool PatternEvaluator::handlePatternLHSQuoteIdent(
    Synonym& syn, std::string lhs, ExpressionSpec& pattern_rhs) {
  // pattern a ("x", <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a ("x", _) --> all assignments with LHS "x"
    auto allowed_lines =
        pkb->getLineForAssignVar(lhs).value_or(std::vector<std::string>());
    return ConstraintSolver::makeAllowedValues(syn, allowed_lines);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a ("x", _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;
    auto matching_assigns =
        pkb->getPartialMatchLinesWithVar(lhs, rhs_partial.str())
            .value_or(std::vector<std::string>());
    std::cout << "Running this case\n";
    return ConstraintSolver::makeAllowedValues(syn, matching_assigns);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

AllowedValuesPairOrBool PatternEvaluator::handlePatternLHSSynonym(
    Synonym& syn, Synonym& lhs, ExpressionSpec& pattern_rhs) {
  // pattern a (v, <...>)
  // Get all variables so that we can make this query
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (v, _) --> all assignments
    auto allowed_values = pkb->getAllPatternLinesAndVars();
    AllowedValuePairSet avs(allowed_values.begin(), allowed_values.end());
    return ConstraintSolver::makeAllowedValues(syn, lhs, avs);
    // assert(false);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (v, _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;

    // Constrain (a,v) together
    auto allowed_values = pkb->getPartialMatchLinesAndVars(rhs_partial.str())
                              .value_or(std::vector<SynonymPair>());

    AllowedValuePairSet avs(allowed_values.begin(), allowed_values.end());
    return ConstraintSolver::makeAllowedValues(syn, lhs, avs);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}