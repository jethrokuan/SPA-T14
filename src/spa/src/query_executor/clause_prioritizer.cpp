#include "query_executor/clause_prioritizer.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/clause_functions.cpp"

std::vector<Clause> ClausePrioritizer::getClauses() {
  // Not worth sorting - little to no gain
  if (tooFewClauses()) return getClausesFromQuery();

  // Initializes all clauses to a default starting score and group
  auto weighted_grouped_clauses = getInitialWeightedGroupedClauses();

  prioritizeClauses(weighted_grouped_clauses);

  return getClausesFromWeightedGroupedClauses(weighted_grouped_clauses);
}

void ClausePrioritizer::prioritizeClauses(
    std::vector<WeightedGroupedClause>& clauses) {
  /* Prioritization algorithm: https://github.com/jethrokuan/SPA-T14/issues/200
   * 1. Apply weightages on clauses: different penalties and modifiers.
   * 2. Group clauses: sort them so that groups are executed one after another
   */
  weightClauses(clauses);
  std::sort(clauses.begin(), clauses.end());
}

// Weight calculation
void ClausePrioritizer::weightClauses(
    std::vector<WeightedGroupedClause>& clauses) {
  // Apply functions over each clause to test if they need to have their weights
  // adjusted, and adjust them if so
  for (auto& clause : clauses) {
    for (auto weightFunction : weightUpdaters) {
      weightFunction(clause);
    }
  }
}

// Actual definition of the weightUpdaters static variables
// These are the functions run to re-weight the clauses
std::vector<WeightFunction> ClausePrioritizer::weightUpdaters = {
    weightBooleanClause, weightAffectsClause,       weightAffectsTClause,
    weightNextTClause,   weightPartialPatternMatch, weightCompletePatternMatch};

std::vector<WeightedGroupedClause>
ClausePrioritizer::getInitialWeightedGroupedClauses() {
  std::vector<WeightedGroupedClause> clauses;
  // Add all clauses to a vector of them
  for (auto& rel_cond : *(query->rel_conds)) {
    clauses.push_back({rel_cond, STARTING_WEIGHT, DEFAULT_GROUP});
  }

  for (auto& pattern : *(query->pattern_conds)) {
    clauses.push_back({pattern, STARTING_WEIGHT, DEFAULT_GROUP});
  }

  for (auto& with_cond : *(query->with_conds)) {
    clauses.push_back({with_cond, STARTING_WEIGHT, DEFAULT_GROUP});
  }

  return clauses;
}

std::vector<Clause> ClausePrioritizer::getClausesFromQuery() {
  std::vector<Clause> clauses;
  // Add all clauses to a vector of them
  for (auto& rel_cond : *(query->rel_conds)) {
    clauses.push_back(rel_cond);
  }

  for (auto& pattern : *(query->pattern_conds)) {
    clauses.push_back(pattern);
  }

  for (auto& with_cond : *(query->with_conds)) {
    clauses.push_back(with_cond);
  }

  return clauses;
}

bool ClausePrioritizer::tooFewClauses() {
  size_t total_clauses = query->rel_conds->size() +
                         query->pattern_conds->size() +
                         query->with_conds->size();
  return total_clauses < MIN_CLAUSES_TO_SORT;
}

std::vector<Clause> ClausePrioritizer::getClausesFromWeightedGroupedClauses(
    const std::vector<WeightedGroupedClause>& clauses) {
  std::vector<Clause> out_clauses;
  for (const auto& clause : clauses) {
    out_clauses.push_back(clause.clause);
  }
  return out_clauses;
}
