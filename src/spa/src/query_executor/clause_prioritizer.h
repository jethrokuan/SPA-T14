#pragma once
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
};

class ClausePrioritizer {
 private:
  Query* query;

  //! Initializes all intemediate clause objects with a starting weight
  std::vector<WeightedGroupedClause> getInitialWeightedGroupedClauses();

  //! Takes all clause types from query and puts them all into one vector
  std::vector<Clause> getClausesFromQuery();

  //! Indicates if not worth sorting - little to no gain
  bool tooFewClauses();

  static const int STARTING_WEIGHT = 1;
  static const int DEFAULT_GROUP = 0;
  static const int MIN_CLAUSES_TO_SORT = 2;

 public:
  ClausePrioritizer(Query* query) : query(query){};
  std::vector<Clause> getClauses();
};
