#include "query_evaluator/pql/factor.h"
#include "utils/utils.h"

using namespace Utils;
using namespace QE;

std::optional<DoubleUnderscoreFactor> DoubleUnderscoreFactor::construct(
    const char* s) {
  std::string duf(s);
  return DoubleUnderscoreFactor::construct(duf);
}
std::optional<DoubleUnderscoreFactor> DoubleUnderscoreFactor::construct(
    std::string& s) {
  if (s.size() < 5 || s.front() != '_' || s.back() != '_' || s[1] != '"' ||
      s[s.size() - 2] != '"') {
    // Check that it starts and ends with "_ and _", with at least something in
    // between
    return std::nullopt;
  }

  // Now can check the type of value between underscores
  std::string arg = s.substr(2, s.size() - 4);
  auto syn = Synonym::construct(arg);

  if (syn) {
    return DoubleUnderscoreFactor(syn.value());
  } else if (has_only_digits(arg)) {
    return DoubleUnderscoreFactor(std::stoul(arg));
  } else {
    return std::nullopt;
  }
}