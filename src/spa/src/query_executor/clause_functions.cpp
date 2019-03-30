#include <functional>
#include "query_executor/clause_prioritizer.h"
#include "query_executor/query_executor.h"

using namespace std::placeholders;

// Clause Type Checker + weight functions

// Generic function that we can std::bind to to fill in the arguments we want
void addWeightToClauses(WeightedGroupedClause& wgclause,
                        int relcond_weight_delta, int pattern_weight_delta,
                        int withcond_weight_delta) {
  auto& clause = wgclause.clause;
  std::visit(
      overload{
          // Follows (1,2) --> checks set in O(1) (memory accesss)
          [&](RelCond*) { wgclause.weight += relcond_weight_delta; },
          // pattern a ("x", "x+y") --> significant comparison work
          [&](PatternB*) { wgclause.weight += pattern_weight_delta; },
          // with 2 = 3 or "2" = "3" --> only computation, no memory access
          [&](WithCond*) { wgclause.weight += withcond_weight_delta; },
      },
      clause);
}

// Boolean-type clauses, e.g.
// Follows(1, 2)
// pattern a ("x", "x+y")
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
            // pattern a ("x", "x+y")
            return QueryExecutor::getRefAsBasic(p->getFirstArg()) &&
                   p->getSecondArg() &&
                   std::holds_alternative<Matcher>(p->getSecondArg().value()) &&
                   (!std::get<Matcher>(p->getSecondArg().value()).isPartial);
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
// The numbers represent the weight adjustments for relcond, pattern and with.
WeightFunction weightBooleanClause =
    std::bind(addWeightToClauses, _1, -90, -50, -100);