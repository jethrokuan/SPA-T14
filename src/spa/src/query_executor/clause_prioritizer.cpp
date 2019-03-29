#include "query_executor/clause_prioritizer.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

std::vector<Clause> ClausePrioritizer::getClauses() {
  // Not worth sorting - little to no gain
  if (tooFewClauses()) return getClausesFromQuery();

  // Initializes all clauses to a default starting score and group
  std::vector<WeightedGroupedClause> clauses =
      getInitialWeightedGroupedClauses();
}

// Utils

std::vector<WeightedGroupedClause>
ClausePrioritizer::getInitialWeightedGroupedClauses() {
  std::vector<WeightedGroupedClause> clauses;
  // Add all clauses to a vector of them
  if (!query->rel_cond->empty()) {
    for (auto& rel_cond : *(query->rel_cond)) {
      clauses.push_back({rel_cond, STARTING_WEIGHT, DEFAULT_GROUP});
    }
  }
  if (!query->patternb->empty()) {
    for (auto& pattern : *(query->patternb)) {
      clauses.push_back({pattern, STARTING_WEIGHT, DEFAULT_GROUP});
    }
  }
  if (!query->with_cond->empty()) {
    for (auto& with_cond : *(query->with_cond)) {
      clauses.push_back({with_cond, STARTING_WEIGHT, DEFAULT_GROUP});
    }
  }
  return clauses;
}

std::vector<Clause> ClausePrioritizer::getClausesFromQuery() {
  std::vector<Clause> clauses;
  // Add all clauses to a vector of them
  if (!query->rel_cond->empty()) {
    for (auto& rel_cond : *(query->rel_cond)) {
      clauses.push_back(rel_cond);
    }
  }
  if (!query->patternb->empty()) {
    for (auto& pattern : *(query->patternb)) {
      clauses.push_back(pattern);
    }
  }
  if (!query->with_cond->empty()) {
    for (auto& with_cond : *(query->with_cond)) {
      clauses.push_back(with_cond);
    }
  }
  return clauses;
}

bool ClausePrioritizer::tooFewClauses() {
  size_t total_clauses = (query->rel_cond ? query->rel_cond->size() : 0) +
                         (query->patternb ? query->patternb->size() : 0) +
                         (query->with_cond ? query->with_cond->size() : 0);
  return total_clauses < MIN_CLAUSES_TO_SORT;
}