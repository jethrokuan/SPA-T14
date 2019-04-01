#include <functional>
#include "query_executor/clause_prioritizer.h"
#include "query_executor/query_executor.h"

using namespace std::placeholders;

// Clause Type Checker + weight functions

/*
 *   SETTINGS!
 */
// Generic penalty/reward numbers
constexpr int EXECUTE_ME_FIRST_REWARD = -1000;
constexpr int SMALL_REWARD = -5;
constexpr int SET_LOOKUP_PENALTY = 5;
constexpr int EXECUTE_ME_LAST_PENALTY = 1000;
constexpr int NO_DELTA = 0;

// Pattern penalty for any string matching

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

// Handle the most expensive runtime clauses
bool isAffectsClause(const WeightedGroupedClause& wgclause) {
  if (auto relcond = std::get_if<RelCond*>(&wgclause.clause)) {
    return (*relcond)->relation == Relation::Affects;
  } else {
    return false;
  }
}
WeightFunction weightAffectsClause =
    std::bind(addWeightToClauses, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + AFFECTS_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA});

bool isAffectsTClause(const WeightedGroupedClause& wgclause) {
  if (auto relcond = std::get_if<RelCond*>(&wgclause.clause)) {
    return (*relcond)->relation == Relation::AffectsT;
  } else {
    return false;
  }
}
WeightFunction weightAffectsTClause =
    std::bind(addWeightToClauses, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + AFFECTS_T_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA});

bool isNextTClause(const WeightedGroupedClause& wgclause) {
  if (auto relcond = std::get_if<RelCond*>(&wgclause.clause)) {
    return (*relcond)->relation == Relation::NextT;
  } else {
    return false;
  }
}
WeightFunction weightNextTClause =
    std::bind(addWeightToClauses, _1,
              RelCondWeightDelta{ON_THE_FLY_PENALTY + NEXT_T_PENALTY},
              PatternWeightDelta{NO_DELTA}, WithCondWeightDelta{NO_DELTA});
