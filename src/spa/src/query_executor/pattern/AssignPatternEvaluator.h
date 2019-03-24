#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/pattern/PatternEvaluator.h"

class AssignPatternEvaluator : public PatternEvaluator {
 private:
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSUnderscore()
      override {
    // pattern a (_, _)
    return QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSUnderscore()
      override {
    // pattern a ("x", _)
    return pkb->getLineForAssignVar(*argLeftAsBasic);
  }
  PairedConstraintSet handlePatternLHSSynonymRHSUnderscore() override {
    // pattern a (v, _)
    return pkb->getAllAssignPatternLinesAndVars();
  }

  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSPartialMatch()
      override {
    // pattern a (_, _"v1"_)
    return pkb->getAssignPatternPartialMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSPartialMatch()
      override {
    // pattern a ("x", _"v1"_)
    return pkb->getAssignPatternPartialMatchLinesWithVar(*argLeftAsBasic,
                                                         *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSPartialMatch()
      override {
    // pattern a (v, _"v1"_)
    return pkb->getAssignPatternPartialMatchLinesAndVars(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSUnderscoreRHSCompleteMatch() override {
    // pattern a (_, "v1")
    return pkb->getAssignPatternCompleteMatchLines(*argRightAsExpression);
  }
  std::optional<SingleConstraintSet>
  handlePatternLHSQuoteIdentRHSCompleteMatch() override {
    // pattern a ("x", "v1")
    return pkb->getAssignPatternCompleteMatchLinesWithVar(
        *argLeftAsBasic, *argRightAsExpression);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSCompleteMatch()
      override {
    // pattern a (v, "v1")
    return pkb->getAssignPatternCompleteMatchLinesAndVars(
        *argRightAsExpression);
  }
  std::optional<SingleConstraintSet> handlePatternLHSUnderscoreRHSNull()
      override {
    std::cout << "LHS underscore RHS null not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<SingleConstraintSet> handlePatternLHSQuoteIdentRHSNull()
      override {
    std::cout << "LHS quoteident RHS null not valid for assign-pattern\n";
    assert(false);
  }
  std::optional<PairedConstraintSet> handlePatternLHSSynonymRHSNull() override {
    std::cout << "LHS synonym RHS null not valid for assign-pattern\n";
    assert(false);
  }

 public:
  AssignPatternEvaluator(std::vector<QE::Declaration>* decls,
                         QE::PatternB* pattern, PKBManager* pkb,
                         QueryConstraints& qc)
      : PatternEvaluator(decls, pattern, pkb, qc) {}
};