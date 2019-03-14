#include "query_executor/pattern/PatternEvaluator.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_builder/core/query_preprocessor.h"
#include "query_executor/query_executor.h"

bool PatternEvaluator::evaluate() {
  auto pattern = query->pattern;
  auto pattern_syn = pattern->at(0)->getSynonym();
  auto pattern_lhs = pattern->at(0)->getFirstArg();
  auto pattern_rhs = pattern->at(0)->getSecondArg();

  // Add entire set of values for the pattern synoynm
  QueryExecutor::addAllValuesForVariableToConstraints(query->declarations, pkb,
                                                      pattern_syn.synonym, qc);

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

bool PatternEvaluator::handlePatternLHSUnderscore(
    const Synonym& syn, const ExpressionSpec& pattern_rhs) {
  // pattern a (_, <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (_, _) --> all assignments
    auto all_assigns = QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
    if (all_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, all_assigns);
    return true;
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (_, _"x + y"_) --> partial match, no var constraint
    std::ostringstream stream;
    stream << *duf;
    auto matching_assigns = pkb->getPartialMatchLines(stream.str())
                                .value_or(std::unordered_set<std::string>());
    if (matching_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, matching_assigns);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

bool PatternEvaluator::handlePatternLHSQuoteIdent(
    const Synonym& syn, const std::string lhs,
    const ExpressionSpec& pattern_rhs) {
  // pattern a ("x", <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a ("x", _) --> all assignments with LHS "x"
    auto allowed_lines = pkb->getLineForAssignVar(lhs).value_or(
        std::unordered_set<std::string>());
    if (allowed_lines.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, allowed_lines);
    return true;
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a ("x", _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;
    auto matching_assigns =
        pkb->getPartialMatchLinesWithVar(lhs, rhs_partial.str())
            .value_or(std::unordered_set<std::string>());
    if (matching_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, matching_assigns);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

bool PatternEvaluator::handlePatternLHSSynonym(
    const Synonym& syn, const Synonym& lhs, const ExpressionSpec& pattern_rhs) {
  // Add entire set of values for lhs pattern variable
  QueryExecutor::addAllValuesForVariableToConstraints(query->declarations, pkb,
                                                      lhs.synonym, qc);
  // pattern a (v, <...>)
  // Get all variables so that we can make this query
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (v, _) --> all assignments
    auto allowed_values = pkb->getAllPatternLinesAndVars();
    if (allowed_values.empty()) return false;  // Empty clause
    PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
    qc.addToPairedVariableConstraints(syn.synonym, lhs.synonym, avs);
    return true;
    // assert(false);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (v, _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;

    // Constrain (a,v) together
    auto allowed_values =
        pkb->getPartialMatchLinesAndVars(rhs_partial.str())
            .value_or(
                std::unordered_set<std::pair<Line, Variable>, pair_hash>());
    if (allowed_values.empty()) return false;  // Empty clause
    PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
    qc.addToPairedVariableConstraints(syn.synonym, lhs.synonym, avs);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}