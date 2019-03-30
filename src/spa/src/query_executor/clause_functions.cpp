#include <functional>
#include "query_executor/clause_prioritizer.h"
#include "query_executor/query_executor.h"

using namespace std::placeholders;

// Clause Type Checker + weight functions

/*
 *   SETTINGS!
 */
// Generic penalty/reward numbers
const int EXECUTE_ME_FIRST_REWARD = -1000;
const int SMALL_REWARD = -5;
const int SET_LOOKUP_PENALTY = 5;
const int EXECUTE_ME_LAST_PENALTY = 1000;

// Pattern penalty for any string matching

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

// Generic function that we can std::bind to to fill in the arguments we want
void addWeightToClauses(WeightedGroupedClause& wgclause,
                        const RelCondWeightDelta& relcond_weight_delta,
                        const PatternWeightDelta& pattern_weight_delta,
                        const WithCondWeightDelta& withcond_weight_delta) {
  auto& clause = wgclause.clause;
  std::visit(
      overload{
          [&](RelCond*) { wgclause.weight += relcond_weight_delta.weight; },
          [&](PatternB*) { wgclause.weight += pattern_weight_delta.weight; },
          [&](WithCond*) { wgclause.weight += withcond_weight_delta.weight; },
      },
      clause);
}

// Boolean-type clauses, e.g.
// Follows(1, 2)
// pattern a ("x", _)
// with 2 = 3
bool isBooleanClause(const WeightedGroupedClause& wgclause) {
  auto& clause = wgclause.clause;
  return std::visit(
      overload{
          // Follows (1,2)
          [](RelCond* r) {
            return QueryExecutor::getRefAsBasic(r->arg1) &&
                   QueryExecutor::getRefAsBasic(r->arg2);
          },
          [](PatternB* p) {
            // pattern a ("x", _) / i("x", _, _) / w("x", _)
            return QueryExecutor::getRefAsBasic(p->getFirstArg()) &&
                   p->getSecondArg() &&
                   std::holds_alternative<Underscore>(
                       p->getSecondArg().value());
          },
          // with 2 = 3 or "2" = "3"
          [](WithCond* w) {
            return (std::get_if<QuoteIdent>(&w->ref1.attr) &&
                    std::get_if<QuoteIdent>(&w->ref2.attr)) ||
                   (std::get_if<unsigned int>(&w->ref1.attr) &&
                    std::get_if<unsigned int>(&w->ref2.attr));
          },
      },
      clause);
}
// WithCond is the easiest to execute (2 = 2), RelCond and Pattern are O(1)
WeightFunction weightBooleanClause =
    std::bind(addWeightToClauses, _1,
              RelCondWeightDelta{EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY},
              PatternWeightDelta{EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY},
              WithCondWeightDelta{EXECUTE_ME_FIRST_REWARD});