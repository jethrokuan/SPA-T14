#pragma once
#include <iostream>
#include <optional>

#include "query_builder/core/exceptions.h"
#include "query_builder/pql/matcher.h"
#include "query_builder/pql/ref.h"
#include "query_builder/pql/synonym.h"
#include "query_builder/pql/underscore.h"
#include "simple_parser/exceptions.h"
#include "simple_parser/interface.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {
//! Represents a pattern clause in PQL (e.g. pattern a (_,"v")

using Expression = std::variant<Underscore, Matcher>;

class PatternCond {
 private:
  Synonym synonym;
  Ref firstArg;
  std::optional<Expression> secondArg;

 public:
  bool isPartial;
  PatternCond(Synonym s, Ref a1, Expression a2)
      : synonym(s), firstArg(a1), secondArg(a2), isPartial(false){};
  PatternCond(Synonym s, Ref a1)
      : synonym(s), firstArg(a1), secondArg(std::nullopt), isPartial(false){};
  PatternCond(Synonym s, Ref a1, Expression a2, bool isPartial_)
      : synonym(s), firstArg(a1), secondArg(a2), isPartial(isPartial_){};

  Synonym getSynonym() const { return synonym; }
  Ref getFirstArg() const { return firstArg; }
  std::optional<Expression> getSecondArg() const { return secondArg; }

  bool operator==(const PatternCond& pat) const {
    return synonym == pat.synonym && firstArg == pat.firstArg &&
           secondArg == pat.secondArg;
  }
};

}  // namespace QE
