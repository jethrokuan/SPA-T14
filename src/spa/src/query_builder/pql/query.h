#pragma once
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/pattern.h"
#include "query_builder/pql/patternb.h"
#include "query_builder/pql/relcond.h"
#include "query_builder/pql/result.h"
#include "query_builder/pql/suchthat.h"

namespace QE {
//! Query object containing all relevant information to pass to the PKB
class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  Result* result;  // Result clause (boolean or tuple)
  // No std::optional (is in C++17 - have to use nullable types)
  std::vector<SuchThat*>* such_that;
  std::vector<RelCond*>* rel_cond;
  std::vector<Pattern*>* pattern;
  std::vector<PatternB*>* patternb;

  // -- Accessors and constructors --

  Query();
  ~Query();
};
}  // namespace QE
