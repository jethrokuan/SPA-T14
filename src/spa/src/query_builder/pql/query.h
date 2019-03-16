#pragma once
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/patternb.h"
#include "query_builder/pql/relcond.h"
#include "query_builder/pql/result.h"

namespace QE {
//! Query object containing all relevant information to pass to the PKB
class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  Result* result;  // Result clause (boolean or tuple)
  // No std::optional (is in C++17 - have to use nullable types)
  std::vector<RelCond*>* rel_cond;
  std::vector<PatternB*>* patternb;

  // -- Accessors and constructors --

  Query();
  ~Query();

  // -- Printing --
  friend std::ostream& operator<<(std::ostream& os, Query const& query) {
    for (const auto& decl : *(query.declarations)) {
      os << decl << "; ";
    }

    for (const auto& relcond : *(query.rel_cond)) {
      os << *relcond << " ";
    }
    for (const auto& pat : *(query.patternb)) {
      os << pat;
    }
    return os;
  }
};
}  // namespace QE
