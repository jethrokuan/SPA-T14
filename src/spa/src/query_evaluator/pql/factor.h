#pragma once
#include <iostream>
#include <optional>
#include <variant>

#include "query_evaluator/pql/synonym.h"
#include "query_evaluator/pql/util.h"

namespace QE {
using Factor = std::variant<Synonym, unsigned int>;
class DoubleUnderscoreFactor {
 private:
  // Make constructors private
  DoubleUnderscoreFactor(Factor factor) : factor(factor) {}

 public:
  Factor factor;
  // Constructs a guaranteed valid Synonym
  static std::optional<DoubleUnderscoreFactor> construct(std::string&);
  static std::optional<DoubleUnderscoreFactor> construct(const char*);

  bool operator==(const DoubleUnderscoreFactor& duf) const {
    return factor == duf.factor;
  }
  friend std::ostream& operator<<(std::ostream& os,
                                  DoubleUnderscoreFactor const& duf) {
    os << streamer{duf.factor};
    return os;
  }
};
}  // namespace QE