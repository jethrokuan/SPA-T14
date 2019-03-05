#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
// #include "query_executor/constraint_solver/constraint_solver.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class FollowsTEvaluator : public SuchThatEvaluator {
 public:
  FollowsTEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  // Handle cases with at least one variable selected

  std::vector<std::string> handleLeftVarSelectedRightBasic(
      std::string& basic_value) override {
    // Follows*(s, 3)
    return pkb->getBeforeLineS(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightVarSelectedLeftBasic(
      std::string& basic_value) override {
    // Follows*(3, s)
    return pkb->getFollowingLineS(basic_value)
        .value_or(std::vector<std::string>());
  }
  bool handleLeftVarSelectedRightUnderscore(std::string& arg_value) override {
    // Follows*(s, _) (for each s)
    return pkb->getFollowingLineS(arg_value) ? true : false;
  }
  bool handleRightVarSelectedLeftUnderscore(std::string& arg_value) override {
    // Follows*(_, s) (for each s)
    return pkb->getBeforeLineS(arg_value) ? true : false;
  }
  bool handleLeftVarSelectedRightVarUnselected(
      std::string& arg_select, std::string& arg_unselect) override {
    // Follows*(s, s1)
    return pkb->isLineFollowLineS(arg_select, arg_unselect) ? true : false;
  }

  bool handleRightVarSelectedLeftVarUnselected(
      std::string& arg_unselect, std::string& arg_select) override {
    // Follows*(s1, s)
    return pkb->isLineFollowLineS(arg_unselect, arg_select) ? true : false;
  }

  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleBothVarsUnselected(std::string& left_arg,
                                std::string& right_arg) override {
    // Follows*(s1, s2)
    return pkb->isLineFollowLineS(left_arg, right_arg) ? true : false;
  }
  std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string& arg) override {
    // Follows*(s1, 3)
    return handleLeftVarSelectedRightBasic(arg);
  }
  std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string& arg) override {
    // Follows*(3, s1)
    return handleRightVarSelectedLeftBasic(arg);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Follows*(3, _)
    return pkb->getFollowingLineS(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Follows*(_, 3)
    return pkb->getBeforeLineS(arg).has_value();
  }
  bool handleLeftVarUnselectedRightUnderscore(std::string& arg) override {
    // Follows*(s1, _) --> is there a statement that is followed by anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSelectedRightUnderscore(arg);
  }
  bool handleRightVarUnselectedLeftUnderscore(std::string& arg) override {
    // Follows*(_, s1) --> is there a statement that follows anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSelectedLeftUnderscore(arg);
  }
  bool handleDoubleBasic(std::string& arg1, std::string& arg2) override {
    // Follows*(2, 3)?
    return pkb->isLineFollowLineS(arg1, arg2);
  }
};