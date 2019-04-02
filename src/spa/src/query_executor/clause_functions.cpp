#include <functional>
#include "query_executor/clause_prioritizer.h"
#include "query_executor/query_executor.h"

using namespace std::placeholders;

// Clause Type Checker + weight functions

/*
 *   SETTINGS!
 */
// Generic penalty/reward numbers
constexpr int EXECUTE_ME_FIRST_REWARD = -10000;
constexpr int SET_LOOKUP_PENALTY = 5;
constexpr int EXECUTE_ME_LAST_PENALTY = 10000;
constexpr int NO_DELTA = 0;

// Pattern penalty for any string matching
constexpr int PARTIAL_STRING_MATCH_PENALTY = 600;
constexpr int COMPLETE_STRING_MATCH_PENALTY = 300;

// On-the-fly penalties
constexpr int ON_THE_FLY_PENALTY = EXECUTE_ME_LAST_PENALTY;
constexpr int AFFECTS_T_PENALTY = 1000;
constexpr int AFFECTS_PENALTY = 500;
constexpr int NEXT_T_PENALTY = 300;

// Make passing into functions more explicit: otherwise magic numbers everywhere
struct RelCondWeightDelta {
  int weight;
};
struct PatternWeightDelta {
  int weight;
};
struct WithCondWeightDelta {
  int weight;
};

//! Add the stated weight to the clause if the matcher returns true
// This function will be used by partial binding of all arguments except
// wgclause
void addWeightToClausesConditionally(WeightedGroupedClause& wgclause,
                                     RelCondWeightDelta& relcond_weight_delta,
                                     PatternWeightDelta& pattern_weight_delta,
                                     WithCondWeightDelta& withcond_weight_delta,
                                     RelCondMatcher& relcond_matcher,
                                     PatternCondMatcher& pattern_matcher,
                                     WithCondMatcher& withcond_matcher) {
  auto& clause = wgclause.clause;
  std::visit(
      overload{
          [&](RelCond* r) {
            if (relcond_matcher(r))
              wgclause.weight += relcond_weight_delta.weight;
          },
          [&](PatternCond* p) {
            if (pattern_matcher(p))
              wgclause.weight += pattern_weight_delta.weight;
          },
          [&](WithCond* w) {
            if (withcond_matcher(w))
              wgclause.weight += withcond_weight_delta.weight;
          },
      },
      clause);
}

// Default matchers that just don't match on each of the types of clause
RelCondMatcher falseRelCondMatcher = [](const RelCond*) { return false; };
PatternCondMatcher falsePatternMatcher = [](const PatternCond*) {
  return false;
};
WithCondMatcher falseWithCondMatcher = [](const WithCond*) { return false; };

// Match basic clauses for each of the clause types (cheap clauses to exec)
RelCondMatcher relCondBasicMatcher = [](const RelCond* r) {
  // Follows (1,2)
  return QueryExecutor::getRefAsBasic(r->arg1) &&
         QueryExecutor::getRefAsBasic(r->arg2);
};
PatternCondMatcher patternBasicMatcher = [](const PatternCond* p) {
  // pattern a ("x", _) / i("x", _, _) / w("x", _)
  return QueryExecutor::getRefAsBasic(p->getFirstArg()) && p->getSecondArg() &&
         std::holds_alternative<Underscore>(p->getSecondArg().value());
};
WithCondMatcher withCondBasicMatcher = [](const WithCond* w) {
  // with 2 = 3 or "2" = "3"
  return (std::get_if<QuoteIdent>(&w->ref1.attr) &&
          std::get_if<QuoteIdent>(&w->ref2.attr)) ||
         (std::get_if<unsigned int>(&w->ref1.attr) &&
          std::get_if<unsigned int>(&w->ref2.attr));
};

// Clauses to check if a relcond is of a particular type
auto isRelCondRelationType(const RelCond* clause, Relation relation) {
  return clause->relation == relation;
}
RelCondMatcher isAffectsClause =
    std::bind(isRelCondRelationType, _1, Relation::Affects);
RelCondMatcher isAffectsTClause =
    std::bind(isRelCondRelationType, _1, Relation::AffectsT);
RelCondMatcher isNextTClause =
    std::bind(isRelCondRelationType, _1, Relation::NextT);

// For penalizing string matching for pattern
PatternCondMatcher patternPartialMatcher = [](const PatternCond* p) {
  auto pattern_rhs = p->getSecondArg();
  auto argRightIsMatcher =
      pattern_rhs && std::holds_alternative<Matcher>(pattern_rhs.value());
  auto argRightIsPartialMatch =
      argRightIsMatcher && std::get<Matcher>(pattern_rhs.value()).isPartial;
  return argRightIsPartialMatch;
};
PatternCondMatcher patternCompleteMatcher = [](const PatternCond* p) {
  auto pattern_rhs = p->getSecondArg();
  auto argRightIsMatcher =
      pattern_rhs && std::holds_alternative<Matcher>(pattern_rhs.value());
  auto argRightIsCompleteMatch =
      argRightIsMatcher && !std::get<Matcher>(pattern_rhs.value()).isPartial;
  return argRightIsCompleteMatch;
};

// *** START OF ACTUAL WEIGHT APPLICATION FUNCTIONS ***

// WithCond is the easiest to execute (2 = 2), RelCond and Pattern are O(1)
WeightFunction weightBooleanClause =
    std::bind(addWeightToClausesConditionally, _1,
              RelCondWeightDelta{EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY},
              PatternWeightDelta{EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY},
              WithCondWeightDelta{EXECUTE_ME_FIRST_REWARD}, relCondBasicMatcher,
              patternBasicMatcher, withCondBasicMatcher);

WeightFunction weightAffectsClause =
    std::bind(addWeightToClausesConditionally, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + AFFECTS_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA},
              isAffectsClause, falsePatternMatcher, falseWithCondMatcher);

WeightFunction weightAffectsTClause =
    std::bind(addWeightToClausesConditionally, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + AFFECTS_T_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA},
              isAffectsTClause, falsePatternMatcher, falseWithCondMatcher);

WeightFunction weightNextTClause =
    std::bind(addWeightToClausesConditionally, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + NEXT_T_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA},
              isNextTClause, falsePatternMatcher, falseWithCondMatcher);

WeightFunction weightPartialPatternMatch =
    std::bind(addWeightToClausesConditionally, _1, RelCondWeightDelta{NO_DELTA},
              PatternWeightDelta{PARTIAL_STRING_MATCH_PENALTY},
              WithCondWeightDelta{NO_DELTA}, falseRelCondMatcher,
              patternPartialMatcher, falseWithCondMatcher);

WeightFunction weightCompletePatternMatch =
    std::bind(addWeightToClausesConditionally, _1, RelCondWeightDelta{NO_DELTA},
              PatternWeightDelta{COMPLETE_STRING_MATCH_PENALTY},
              WithCondWeightDelta{NO_DELTA}, falseRelCondMatcher,
              patternCompleteMatcher, falseWithCondMatcher);
