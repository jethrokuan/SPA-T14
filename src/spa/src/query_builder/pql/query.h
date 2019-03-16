#pragma once
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/patternb.h"
#include "query_builder/pql/relcond.h"
#include "query_builder/pql/result.h"
#include "query_builder/pql/withcond.h"

namespace QE {
//! Query object containing all relevant information to pass to the PKB
class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  Result* result;  // Result clause (boolean or tuple)
  std::vector<RelCond*>* rel_cond;
  std::vector<PatternB*>* patternb;
  std::vector<WithCond*>* with_cond;

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

    // TODO: print WithCond
    return os;
  }
};
}  // namespace QE
