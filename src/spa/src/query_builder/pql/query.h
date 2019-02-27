#pragma once
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/pattern.h"
#include "query_builder/pql/suchthat.h"

namespace QE {
//! Query object containing all relevant information to pass to the PKB
class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  // Selected declaration refers to the synonym that is after the 'Select'
  Declaration* selected_declaration;
  // No std::optional (is in C++17 - have to use nullable types)
  SuchThat* such_that;
  Pattern* pattern;

  // -- Accessors and constructors --

  Query();
  ~Query();
};
}  // namespace QE