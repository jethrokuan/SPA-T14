#pragma once
#include <iostream>
#include <optional>
#include <regex>

namespace QE {
//! Represents a PQL synonym (LETTER (LETTER | DIGIT), e.g. x2002y)
class Synonym {
 public:
  Synonym(std::string synonym);
  std::string synonym;

  static bool isValidSynonym(std::string str);
  bool operator==(const Synonym& a2) const { return synonym == a2.synonym; }
  friend std::ostream& operator<<(std::ostream& os, Synonym const& syn) {
    os << syn.synonym;
    return os;
  }
};
}  // namespace QE
