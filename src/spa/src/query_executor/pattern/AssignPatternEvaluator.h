#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/pattern/PatternEvaluator.h"

class AssignPatternEvaluator : public PatternEvaluator {
 private:
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSUnderscore() {
    return QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSUnderscore() {
    return pkb->getLineForAssignVar(*argLeftAsBasic);
  }
  PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() {
    return pkb->getAllAssignPatternLinesAndVars();
  }

  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSPartialMatch() {
    return pkb->getAssignPatternPartialMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSPartialMatch() {
    return pkb->getAssignPatternPartialMatchLinesWithVar(*argLeftAsBasic,
                                                         *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSPartialMatch() {
    return pkb->getAssignPatternPartialMatchLinesAndVars(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() {
    return pkb->getAssignPatternCompleteMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() {
    return pkb->getAssignPatternCompleteMatchLinesWithVar(
        *argLeftAsBasic, *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSCompleteMatch() {
    return pkb->getAssignPatternCompleteMatchLinesAndVars(
        *argRightAsExpression);
  }
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSNull() {
    std::cout << "LHS underscore RHS null not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSNull() {
    std::cout << "LHS quoteident RHS null not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSSynonymRHSNull() {
    std::cout << "LHS synonym RHS null not valid for assign-pattern\n";
    assert(false);
  }

 public:
  AssignPatternEvaluator(std::vector<QE::Declaration>* decls,
                         QE::PatternB* pattern, PKBManager* pkb,
                         QueryConstraints& qc)
      : PatternEvaluator(decls, pattern, pkb, qc) {}
};