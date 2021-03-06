#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/pattern/PatternEvaluator.h"

class IfPatternEvaluator : public PatternEvaluator {
 private:
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSUnderscore()
      override {
    std::cout << "LHS Pattern RHS underscore not valid for assign-if\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSUnderscore()
      override {
    std::cout << "LHS QuoteIdent RHS underscore not valid for assign-if\n";
    assert(false);
  }
  PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() override {
    std::cout << "LHS Underscore RHS underscore not valid for assign-if\n";
    assert(false);
  }

  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSPartialMatch()
      override {
    std::cout << "LHS Underscore RHS partial not valid for assign-if\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSPartialMatch()
      override {
    std::cout << "LHS QuoteIdent RHS partial not valid for assign-if\n";
    assert(false);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSPartialMatch()
      override {
    std::cout << "LHS Synonym RHS partial not valid for assign-if\n";
    assert(false);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() override {
    std::cout << "LHS Underscore RHS complete not valid for assign-if\n";
    assert(false);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() override {
    std::cout << "LHS QuoteIdent RHS complete not valid for assign-if\n";
    assert(false);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSCompleteMatch()
      override {
    std::cout << "LHS Synonym RHS complete not valid for assign-if\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSNull()
      override {
    // pattern ifs (_, _, _)
    // Get only the lines from the lines & vars
    // There can be if statements without a control variable
    PairedConstraintSet allLinesVars = pkb->getAllIfPatternLinesAndVars();
    SingleConstraintSet lines;
    for (const auto& [line, var] : allLinesVars) {
      lines.insert(line);
    }
    return lines;
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSNull()
      override {
    // pattern ifs ("x", _, _)
    return pkb->getIfPatternLine(*argLeftAsBasic);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSNull() override {
    // pattern ifs (v, _, _)
    return pkb->getAllIfPatternLinesAndVars();
  }

 public:
  IfPatternEvaluator(std::vector<QE::Declaration>* decls,
                     QE::PatternCond* pattern, PKBManager* pkb,
                     ConstraintDatabase& db)
      : PatternEvaluator(decls, pattern, pkb, db) {}
};