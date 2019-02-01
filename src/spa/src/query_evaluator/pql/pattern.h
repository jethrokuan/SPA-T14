#pragma once
#include <iostream>
#include <optional>

#include "query_evaluator/pql/factor.h"
#include "query_evaluator/pql/stmtentref.h"
#include "query_evaluator/pql/synonym.h"
#include "query_evaluator/pql/underscore.h"
#include "query_evaluator/pql/util.h"

namespace QE {
using ExpressionSpec = std::variant<DoubleUnderscoreFactor, Underscore>;

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
