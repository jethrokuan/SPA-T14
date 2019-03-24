#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/pattern/PatternEvaluator.h"

class WhilePatternEvaluator : public PatternEvaluator {
 private:
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSUnderscore() {
    std::cout << "LHS Pattern RHS underscore not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSUnderscore() {
    std::cout << "LHS QuoteIdent RHS underscore not valid for assign-pattern\n";
    assert(false);
  }
  PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() {
    std::cout << "LHS Underscore RHS underscore not valid for assign-pattern\n";
    assert(false);
  }

  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSPartialMatch() {
    std::cout << "LHS Underscore RHS partial not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSPartialMatch() {
    std::cout << "LHS QuoteIdent RHS partial not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSPartialMatch() {
    std::cout << "LHS Synonym RHS partial not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() {
    std::cout << "LHS Underscore RHS complete not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() {
    std::cout << "LHS QuoteIdent RHS complete not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSCompleteMatch() {
    std::cout << "LHS Synonym RHS complete not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSNull() {
    // pattern ifs (_, _, _)
    return QueryExecutor::getSelect(pkb, DesignEntity::WHILE);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSNull() {
    // pattern ifs ("x", _, _)
    return pkb->getWhilePatternLine(*argLeftAsBasic);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSNull() {
    // pattern ifs ("x", _, _)
    return pkb->getAllWhilePatternLinesAndVars();
  }

 public:
  WhilePatternEvaluator(std::vector<QE::Declaration>* decls,
                        QE::PatternB* pattern, PKBManager* pkb,
                        QueryConstraints& qc)
      : PatternEvaluator(decls, pattern, pkb, qc) {}
};