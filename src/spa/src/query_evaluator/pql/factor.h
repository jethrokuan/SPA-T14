#pragma once
#include <iostream>
#include <optional>
#include <variant>

#include "query_evaluator/pql/synonym.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {
//! A factor can be either a synonym or a statement number
using Factor = std::variant<Synonym, unsigned int>;
/*!
 *  \brief Represents a PQL factor term that is surrounded by two underscores.
 *         (e.g. _"x"_)
 */
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