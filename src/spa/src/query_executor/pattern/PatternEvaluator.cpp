#include "query_executor/pattern/PatternEvaluator.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "query_executor/query_executor.h"

bool PatternEvaluator::evaluate() {
  // Add entire set of values for the pattern synoynm
  QueryExecutor::addAllValuesForVariableToConstraints(declarations, pkb,
                                                      synonym, qc);

  // Compute all derived expression w.r.t pattern arguments

  argLeftIsUnderscore = std::holds_alternative<Underscore>(pattern_lhs);
  argLeftIsQuoteIdent = std::holds_alternative<QuoteIdent>(pattern_lhs);
  argLeftIsSynonym = std::holds_alternative<Synonym>(pattern_lhs);

  argRightIsNull = !pattern_rhs;
  argRightIsUnderscore =
      pattern_rhs && std::holds_alternative<Underscore>(pattern_rhs.value());
  argRightIsMatcher =
      pattern_rhs && std::holds_alternative<Matcher>(pattern_rhs.value());
  argRightIsPartialMatch =
      argRightIsMatcher && std::get<Matcher>(pattern_rhs.value()).isPartial;
  argRightIsCompleteMatch =
      argRightIsMatcher && !std::get<Matcher>(pattern_rhs.value()).isPartial;

  argLeftAsBasic = QueryExecutor::getRefAsBasic(pattern_lhs);
  argLeftAsSynonym = QueryExecutor::getRefAsSynonym(pattern_lhs);

  argRightAsExpression =
      argRightIsMatcher
          ? std::make_optional(std::get<Matcher>(pattern_rhs.value()).expr)
          : std::nullopt;

  return dispatch();
}

bool PatternEvaluator::dispatch() {
  // TODO: More cases for pattern in iteration 2
  // Clearly: 6 possible cases (cross product of re and expression)
  // Match on ref
  if (argLeftIsUnderscore) {
    if (argRightIsUnderscore) {
      return dispatchPatternLHSUnderscoreRHSUnderscore();
    } else if (argRightIsNull) {
      return dispatchPatternLHSUnderscoreRHSNull();
    } else if (argRightIsPartialMatch) {
      return dispatchPatternLHSUnderscoreRHSPartialMatch();
    } else if (argRightIsCompleteMatch) {
      return dispatchPatternLHSUnderscoreRHSCompleteMatch();
    }
  } else if (argLeftIsQuoteIdent) {
    if (argRightIsUnderscore) {
      return dispatchPatternLHSQuoteIdentRHSUnderscore();
    } else if (argRightIsNull) {
      return dispatchPatternLHSQuoteIdentRHSNull();
    } else if (argRightIsPartialMatch) {
      return dispatchPatternLHSQuoteIdentRHSPartialMatch();
    } else if (argRightIsCompleteMatch) {
      return dispatchPatternLHSQuoteIdentRHSCompleteMatch();
    }
  } else if (argLeftIsSynonym) {
    if (argRightIsUnderscore) {
      return dispatchPatternLHSSynonymRHSUnderscore();
    } else if (argRightIsNull) {
      return dispatchPatternLHSSynonymRHSNull();
    } else if (argRightIsPartialMatch) {
      return dispatchPatternLHSSynonymRHSPartialMatch();
    } else if (argRightIsCompleteMatch) {
      return dispatchPatternLHSSynonymRHSCompleteMatch();
    }
  } else {
    assert(false);
  }
}

bool PatternEvaluator::dispatchPatternLHSUnderscoreRHSUnderscore() {
  // pattern a (_, _) --> all assignments
  auto all_assigns = QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
  if (all_assigns.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, all_assigns);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSQuoteIdentRHSUnderscore() {
  // pattern a ("x", _) --> all assignments with LHS "x"
  auto allowed_lines = pkb->getLineForAssignVar(*argLeftAsBasic)
                           .value_or(std::unordered_set<std::string>());
  if (allowed_lines.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, allowed_lines);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSSynonymRHSUnderscore() {
  // pattern a (v, _) --> all assignments
  auto allowed_values = pkb->getAllAssignPatternLinesAndVars();
  if (allowed_values.empty()) return false;  // Empty clause
  PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
  qc.addToPairedVariableConstraints(synonym, argLeftAsSynonym->synonym, avs);
  return true;
}

bool PatternEvaluator::dispatchPatternLHSUnderscoreRHSPartialMatch() {
  // pattern a (_, _"x + y"_) --> partial match, no var constraint
  SingleConstraintSet matching_assigns;
  matching_assigns =
      pkb->getAssignPatternPartialMatchLines(*argRightAsExpression)
          .value_or(std::unordered_set<std::string>());
  if (matching_assigns.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, matching_assigns);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSQuoteIdentRHSPartialMatch() {
  // pattern a ("x", _"x + y"_) --> partial match
  SingleConstraintSet matching_assigns;
  matching_assigns = pkb->getAssignPatternPartialMatchLinesWithVar(
                            *argLeftAsBasic, *argRightAsExpression)
                         .value_or(SingleConstraintSet());

  if (matching_assigns.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, matching_assigns);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSSynonymRHSPartialMatch() {
  // pattern a (v, _"x + y"_) --> partial match
  // Select partial or complete match
  PairedConstraintSet allowed_values;
  allowed_values =
      pkb->getAssignPatternPartialMatchLinesAndVars(*argRightAsExpression)
          .value_or(PairedConstraintSet());
  if (allowed_values.empty()) return false;  // Empty clause
  PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
  qc.addToPairedVariableConstraints(synonym, argLeftAsSynonym->synonym, avs);
  return true;
}

bool PatternEvaluator::dispatchPatternLHSUnderscoreRHSCompleteMatch() {
  // pattern a (_, "x + y") --> complete match
  SingleConstraintSet matching_assigns;
  matching_assigns =
      pkb->getAssignPatternCompleteMatchLines(*argRightAsExpression)
          .value_or(std::unordered_set<std::string>());

  if (matching_assigns.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, matching_assigns);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSQuoteIdentRHSCompleteMatch() {
  // pattern a ("x", _"x + y"_) --> complete match
  SingleConstraintSet matching_assigns;
  matching_assigns = pkb->getAssignPatternCompleteMatchLinesWithVar(
                            *argLeftAsBasic, *argRightAsExpression)
                         .value_or(SingleConstraintSet());

  if (matching_assigns.empty()) return false;  // Empty clause
  qc.addToSingleVariableConstraints(synonym, matching_assigns);
  return true;
}
bool PatternEvaluator::dispatchPatternLHSSynonymRHSCompleteMatch() {
  // pattern a (v, "x + y") --> complete match
  PairedConstraintSet allowed_values;
  allowed_values =
      pkb->getAssignPatternCompleteMatchLinesAndVars(*argRightAsExpression)
          .value_or(PairedConstraintSet());

  if (allowed_values.empty()) return false;  // Empty clause
  PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
  qc.addToPairedVariableConstraints(synonym, argLeftAsSynonym->synonym, avs);
  return true;
}

bool PatternEvaluator::dispatchPatternLHSUnderscoreRHSNull() {}
bool PatternEvaluator::dispatchPatternLHSQuoteIdentRHSNull() {}
bool PatternEvaluator::dispatchPatternLHSSynonymRHSNull() {}

/*
bool PatternEvaluator::dispatch() {
  // TODO: More cases for pattern in iteration 2
  // Clearly: 6 possible cases (cross product of re and expression)
  // Match on ref
  if (argLeftIsUnderscore) {
    return handlePatternLHSUnderscore();
  } else if (argLeftIsQuoteIdent) {
    return handlePatternLHSQuoteIdent(
        synonym, QueryExecutor::getRefAsBasic(pattern_lhs).value(),
        pattern_rhs.value());
  } else if (auto lhs_syn = std::get_if<Synonym>(&pattern_lhs)) {
    return handlePatternLHSSynonym(synonym, *lhs_syn, pattern_rhs.value());
  } else {
    assert(false);
  }
}
*/

/*

bool PatternEvaluator::handlePatternLHSUnderscore() {
  // pattern a (_, <...>)
  if (std::get_if<Underscore>(&pattern_rhs.value())) {
    // pattern a (_, _) --> all assignments
    auto all_assigns = QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
    if (all_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(synonym, all_assigns);
    return true;
  } else if (auto matcher = std::get_if<Matcher>(&pattern_rhs)) {
    // pattern a (_, _"x + y"_) --> partial match, no var constraint
    // TODO: Iter 2 check if matcher is partial!
    // Assume is true for now
    std::ostringstream stream;
    stream << matcher->expr;

    // Select partial or complete match
    SingleConstraintSet matching_assigns;
    if (matcher->isPartial) {
      matching_assigns = pkb->getAssignPatternPartialMatchLines(stream.str())
                             .value_or(std::unordered_set<std::string>());
    } else {
      matching_assigns = pkb->getAssignPatternCompleteMatchLines(stream.str())
                             .value_or(std::unordered_set<std::string>());
    }

    if (matching_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, matching_assigns);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

bool PatternEvaluator::handlePatternLHSQuoteIdent(
    const Synonym& syn, const std::string lhs, const Expression& pattern_rhs) {
  // pattern a ("x", <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a ("x", _) --> all assignments with LHS "x"
    auto allowed_lines = pkb->getLineForAssignVar(lhs).value_or(
        std::unordered_set<std::string>());
    if (allowed_lines.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, allowed_lines);
    return true;
  } else if (auto matcher = std::get_if<Matcher>(&pattern_rhs)) {
    // pattern a ("x", _"x + y"_) --> partial match, no var constraint
    // TODO: Iter 2 check if matcher is partial!
    // Assume is true for now
    std::ostringstream rhs_partial;
    rhs_partial << matcher->expr;

    // Select partial or complete match
    SingleConstraintSet matching_assigns;
    if (matcher->isPartial) {
      matching_assigns =
          pkb->getAssignPatternPartialMatchLinesWithVar(lhs, rhs_partial.str())
              .value_or(SingleConstraintSet());
    } else {
      matching_assigns =
          pkb->getAssignPatternCompleteMatchLinesWithVar(lhs, rhs_partial.str())
              .value_or(SingleConstraintSet());
    }

    if (matching_assigns.empty()) return false;  // Empty clause
    qc.addToSingleVariableConstraints(syn.synonym, matching_assigns);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

bool PatternEvaluator::handlePatternLHSSynonym(const Synonym& syn,
                                               const Synonym& lhs,
                                               const Expression& pattern_rhs) {
  // Add entire set of values for lhs pattern variable
  QueryExecutor::addAllValuesForVariableToConstraints(declarations, pkb,
                                                      lhs.synonym, qc);
  // pattern a (v, <...>)
  // Get all variables so that we can make this query
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (v, _) --> all assignments
    auto allowed_values = pkb->getAllAssignPatternLinesAndVars();
    if (allowed_values.empty()) return false;  // Empty clause
    PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
    qc.addToPairedVariableConstraints(syn.synonym, lhs.synonym, avs);
    return true;
    // assert(false);
  } else if (auto matcher = std::get_if<Matcher>(&pattern_rhs)) {
    // pattern a (v, _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << matcher->expr;

    // Select partial or complete match
    PairedConstraintSet allowed_values;
    if (matcher->isPartial) {
      allowed_values =
          pkb->getAssignPatternPartialMatchLinesAndVars(rhs_partial.str())
              .value_or(PairedConstraintSet());
    } else {
      allowed_values =
          pkb->getAssignPatternCompleteMatchLinesAndVars(rhs_partial.str())
              .value_or(PairedConstraintSet());
    }

    if (allowed_values.empty()) return false;  // Empty clause
    PairedConstraintSet avs(allowed_values.begin(), allowed_values.end());
    qc.addToPairedVariableConstraints(syn.synonym, lhs.synonym, avs);
    return true;
  } else {
    // nothing else allowed for now
    assert(false);
  }
}
*/