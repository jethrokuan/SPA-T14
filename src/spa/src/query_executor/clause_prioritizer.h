#pragma once
#include <functional>
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"

using namespace PKB;
using namespace QE;

struct WeightedGroupedClause {
  Clause clause;
  int weight;             // Lower score ==> higher prority, can be negative
  unsigned int group_id;  // To group related clauses together

  // Sort by groupid, then weight
  bool operator<(const WeightedGroupedClause& other) {
    return std::tie(group_id, weight) < std::tie(other.group_id, other.weight);
  }
  friend std::ostream& operator<<(std::ostream& os,
                                  WeightedGroupedClause const& wgc) {
    os << "Group: " << wgc.group_id << ", Weight: " << wgc.weight;
    return os;
  }
};

//! For application of clause weights based on their type
using ClauseTypeChecker = std::function<bool(const WeightedGroupedClause&)>;
using WeightFunction = std::function<void(WeightedGroupedClause&)>;

class ClausePrioritizer {
 private:
  Query* query;

  //! Initializes all intemediate clause objects with a starting weight
  std::vector<WeightedGroupedClause> getInitialWeightedGroupedClauses();

  //! Takes all clause types from query and puts them all into one vector
  std::vector<Clause> getClausesFromQuery();

  //! Indicates if not worth sorting - little to no gain
  bool tooFewClauses();

  //! Calculates weights and groupings for clauses - modifies input parameter
  void prioritizeClauses(std::vector<WeightedGroupedClause>& clauses);

  //! Does weight assignments / modifications to clauses
  void weightClauses(std::vector<WeightedGroupedClause>& clauses);

  //! Groups related clauses together to keep result table small
  void groupClauses(std::vector<WeightedGroupedClause>& clauses);

  //! Utility to go from weighted clauses back to normal clauses
  std::vector<Clause> getClausesFromWeightedGroupedClauses(
      const std::vector<WeightedGroupedClause>& clauses);

  /*
   * This is a list of functions that test if a clause is of a certain type and
   * then applies a function to update the weights inside that clause if it
   * matches
   */
  static std::vector<std::pair<ClauseTypeChecker, WeightFunction>>
      weightUpdaters;

  static const int STARTING_WEIGHT = 100;
  static const int DEFAULT_GROUP = 0;
  static const int MIN_CLAUSES_TO_SORT = 2;

 public:
  ClausePrioritizer(Query* query) : query(query){};
  std::vector<Clause> getClauses();
};
