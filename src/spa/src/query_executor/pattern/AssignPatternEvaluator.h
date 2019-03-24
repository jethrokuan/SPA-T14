#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/pattern/PatternEvaluator.h"

class AssignPatternEvaluator : public PatternEvaluator {
 private:
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSUnderscore() {
    // pattern a (_, _)
    return QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSUnderscore() {
    // pattern a ("x", _)
    return pkb->getLineForAssignVar(*argLeftAsBasic);
  }
  PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() {
    // pattern a (v, _)
    return pkb->getAllAssignPatternLinesAndVars();
  }

  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSPartialMatch() {
    // pattern a (_, _"v1"_)
    return pkb->getAssignPatternPartialMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSPartialMatch() {
    // pattern a ("x", _"v1"_)
    return pkb->getAssignPatternPartialMatchLinesWithVar(*argLeftAsBasic,
                                                         *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSPartialMatch() {
    // pattern a (v, _"v1"_)
    return pkb->getAssignPatternPartialMatchLinesAndVars(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() {
    // pattern a (_, "v1")
    return pkb->getAssignPatternCompleteMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() {
    // pattern a ("x", "v1")
    return pkb->getAssignPatternCompleteMatchLinesWithVar(
        *argLeftAsBasic, *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSCompleteMatch() {
    // pattern a (v, "v1")
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
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSNull() {
    std::cout << "LHS synonym RHS null not valid for assign-pattern\n";
    assert(false);
  }

 public:
  AssignPatternEvaluator(std::vector<QE::Declaration>* decls,
                         QE::PatternB* pattern, PKBManager* pkb,
                         QueryConstraints& qc)
      : PatternEvaluator(decls, pattern, pkb, qc) {}
};