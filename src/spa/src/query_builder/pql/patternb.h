#pragma once
#include <iostream>
#include <optional>

#include "query_builder/pql/ref.h"
#include "query_builder/pql/synonym.h"
#include "query_builder/pql/underscore.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {
//! Represents a pattern clause in PQL (e.g. pattern a (_,"v")

class Matcher {
 public:
  bool isPartial;
  std::string expr;

  Matcher(bool isPartial_, std::string expr_)
      : isPartial(isPartial_), expr(expr_){};

  bool operator==(const Matcher& other) const {
    return isPartial == other.isPartial && expr.compare(other.expr) == 0;
  }
};

using Expression = std::variant<Underscore, Matcher>;

class PatternB {
 private:
  Synonym synonym;
  Ref firstArg;
  std::optional<Expression> secondArg;

 public:
  bool isPartial;
  PatternB(Synonym s, Ref a1, Expression a2)
      : synonym(s), firstArg(a1), secondArg(a2), isPartial(false){};
  PatternB(Synonym s, Ref a1)
      : synonym(s), firstArg(a1), secondArg(std::nullopt), isPartial(false){};
  PatternB(Synonym s, Ref a1, Expression a2, bool isPartial_)
      : synonym(s), firstArg(a1), secondArg(a2), isPartial(isPartial_){};

  Synonym getSynonym() const { return synonym; }
  Ref getFirstArg() const { return firstArg; }

  bool operator==(const PatternB& pat) const {
    return synonym == pat.synonym && firstArg == pat.firstArg &&
           secondArg == pat.secondArg;
  }
};

}  // namespace QE
