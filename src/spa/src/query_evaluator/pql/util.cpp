#include "query_evaluator/pql/util.h"

namespace QE {
bool has_only_digits(const std::string s) {
  return s.find_first_not_of("0123456789") == std::string::npos;
}
}  // namespace QE