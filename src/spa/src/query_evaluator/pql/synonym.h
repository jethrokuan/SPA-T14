#pragma once
#include <optional>
#include <regex>
#include <iostream>

#include "query_evaluator/pql/pql.h"

namespace QE {
class Synonym {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  static const std::regex synonym_regex;

  // Make constructors private
  Synonym() {}
  Synonym(std::string& synonym) : synonym(synonym) {}

 public:
  std::string synonym;
  // Constructs a guaranteed valid Synonym
  static std::optional<Synonym> construct(std::string&);
  static std::optional<Synonym> construct(const char*);

  bool operator==(const Synonym& a2) const { return synonym == a2.synonym; }
  friend std::ostream& operator<<(std::ostream& os, Synonym const& syn) {
    os << syn.synonym;
    return os;
  }
};
}  // namespace QE