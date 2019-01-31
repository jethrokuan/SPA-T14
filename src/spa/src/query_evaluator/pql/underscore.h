#pragma once
#include <iostream>

namespace QE {
class Underscore {
 public:
  bool operator==(__attribute__((unused)) const Underscore& u) const {
    return true;
  }
  friend std::ostream& operator<<(std::ostream& os,
                                  __attribute__((unused)) Underscore const& u) {
    os << "_";
    return os;
  }
};
}  // namespace QE