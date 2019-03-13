#pragma once
#include <variant>
#include <vector>
#include "query_builder/pql/synonym.h"

namespace QE {
using Elem = Synonym;
enum class ResultType { BOOLEAN, TUPLE };
struct Result {
  ResultType T;
  std::vector<Declaration*>* selected_declarations;
};
}  // namespace QE
