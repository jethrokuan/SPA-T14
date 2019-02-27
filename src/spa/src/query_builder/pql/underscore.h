#pragma once
#include <iostream>

namespace QE {
//! Strongly-typed underscore in a PQL query (i.e. _)
class Underscore {
 public:
  bool operator==(const Underscore&) const { return true; }
  friend std::ostream& operator<<(std::ostream& os, Underscore const&) {
    os << "_";
    return os;
  }
};
}  // namespace QE