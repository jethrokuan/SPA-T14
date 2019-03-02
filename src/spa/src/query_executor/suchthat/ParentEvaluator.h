#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class ParentEvaluator : public SuchThatEvaluator {
 public:
  ParentEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  // Handle cases with at least one variable selected

  std::vector<std::string> handleLeftVarSelectedRightBasic(
      std::string& basic_value) override {
    // Parent(s, 3)
    if (auto beforeLine = pkb->getParentLine(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::vector<std::string> handleRightVarSelectedLeftBasic(
      std::string& basic_value) override {
    // Parent(3, s)
    if (auto afterLine = pkb->getChildLine(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftVarSelectedRightUnderscore(std::string& arg_value) override {
    // Parent(s, _) (for each s)
    return pkb->getChildLine(arg_value) ? true : false;
  }
  bool handleRightVarSelectedLeftUnderscore(std::string& arg_value) override {
    // Parent(_, s) (for each s)
    return pkb->getParentLine(arg_value) ? true : false;
  }
  bool handleLeftVarSelectedRightVarUnselected(
      std::string& arg_select, std::string& arg_unselect) override {
    // Parent(s, s1)
    return pkb->isLineParentLine(arg_select, arg_unselect) ? true : false;
  }

  bool handleRightVarSelectedLeftVarUnselected(
      std::string& arg_unselect, std::string& arg_select) override {
    // Parent(s1, s)
    return pkb->isLineParentLine(arg_unselect, arg_select) ? true : false;
  }

  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleBothVarsUnselected(std::string& left_arg,
                                std::string& right_arg) override {
    // Parent(s1, s2)
    return pkb->isLineParentLine(left_arg, right_arg) ? true : false;
  }
  std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string& arg) override {
    // Parent(s1, 3)
    return handleLeftVarSelectedRightBasic(arg);
  }
  std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string& arg) override {
    // Parent(3, s1)
    return handleRightVarSelectedLeftBasic(arg);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Parent(3, _)
    return pkb->getChildLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Parent(_, 3)
    return pkb->getParentLine(arg).has_value();
  }
  bool handleLeftVarUnselectedRightUnderscore(std::string& arg) override {
    // Parent(s1, _) --> is there a statement that is followed by anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSelectedRightUnderscore(arg);
  }
  bool handleRightVarUnselectedLeftUnderscore(std::string& arg) override {
    // Parent(_, s1) --> is there a statement that follows anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSelectedLeftUnderscore(arg);
  }
};