#pragma once
#include <iostream>
#include <optional>

#include "query_builder/pql/factor.h"
#include "query_builder/pql/stmtentref.h"
#include "query_builder/pql/synonym.h"
#include "query_builder/pql/underscore.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {
//! An ExprSpec term in PQL can be either a DUF or just an Underscore
using ExpressionSpec = std::variant<DoubleUnderscoreFactor, Underscore, Factor>;

//! Represents a pattern clause in PQL (e.g. pattern a (_,"v")
class Pattern {
 private:
  Synonym synonym;
  EntRef firstArg;
  ExpressionSpec secondArg;
  Pattern(Synonym& s, EntRef& a1, ExpressionSpec& a2)
      : synonym(s), firstArg(a1), secondArg(a2){};

 public:
  static std::optional<Pattern> construct(Synonym&, EntRef&, ExpressionSpec&);
  static std::optional<Pattern*> construct_heap(Synonym&, EntRef&,
                                                ExpressionSpec&);
  Synonym getSynonym() const { return synonym; }
  EntRef getFirstArg() const { return firstArg; }
  ExpressionSpec getSecondArg() const { return secondArg; }

  bool operator==(const Pattern& pat) const {
    return synonym == pat.synonym && firstArg == pat.firstArg &&
           secondArg == pat.secondArg;
  }

  friend std::ostream& operator<<(std::ostream& os, Pattern const& pat) {
    os << pat.synonym << "(" << streamer{pat.getFirstArg()} << ", "
       << streamer{pat.getSecondArg()} << ")";
    return os;
  }
};

}  // namespace QE
