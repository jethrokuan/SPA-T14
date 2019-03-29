#pragma once
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"

using namespace PKB;
using namespace QE;

struct WeightedClause {
  Clause clause;
  int weight;
};

class ClausePrioritizer {
 private:
  Query* query;

  //! Initializes all intemediate clause objects with a starting weight
  std::vector<WeightedClause> getInitialWeightedClauses();

  //! Takes all clause types from query and puts them all into one vector
  std::vector<Clause> getClausesFromQuery();

  //! Indicates if not worth sorting - little to no gain
  bool tooFewClauses();

  static const int STARTING_WEIGHT = 1;
  static const int MIN_CLAUSES_TO_SORT = 5;

 public:
  ClausePrioritizer(Query* query) : query(query){};
  std::vector<Clause> getClauses();
};
