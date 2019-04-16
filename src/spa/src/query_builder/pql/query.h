#pragma once
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/pattern_cond.h"
#include "query_builder/pql/rel_cond.h"
#include "query_builder/pql/result.h"
#include "query_builder/pql/with_cond.h"

namespace QE {
//! Utility definition to refer to all three types of clauses
using Clause = std::variant<QE::RelCond*, QE::PatternCond*, QE::WithCond*>;

//! Query object containing all relevant information to pass to the PKB
class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  Result* result;  // Result clause (boolean or tuple)
  std::vector<RelCond*>* rel_conds;
  std::vector<PatternCond*>* pattern_conds;
  std::vector<WithCond*>* with_conds;

  // -- Accessors and constructors --

  Query();
  ~Query();

  // -- Printing --
  friend std::ostream& operator<<(std::ostream& os, Query const& query) {
    for (const auto& decl : *(query.declarations)) {
      os << decl << "; ";
    }

    for (const auto& relcond : *(query.rel_conds)) {
      os << *relcond << " ";
    }
    for (const auto& pat : *(query.pattern_conds)) {
      os << pat;
    }

    // TODO: print WithCond
    return os;
  }
};

void printClause(Clause clause);
}  // namespace QE
