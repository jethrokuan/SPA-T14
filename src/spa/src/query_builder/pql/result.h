#pragma once
#include <variant>
#include <vector>
#include "query_builder/pql/attrref.h"
#include "query_builder/pql/synonym.h"

namespace QE {
enum class ResultType { BOOLEAN, TUPLE };
using ResultItem = std::variant<Synonym, SynAttr>;
struct Result {
  ResultType T;
  std::vector<ResultItem>* selected_declarations;
};
}  // namespace QE
