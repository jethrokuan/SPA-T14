#pragma once
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class UsesSEvaluator : public SuchThatEvaluator {
 public:
  UsesSEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  // Handle cases with at least one variable selected

  // Handle cases with at least one variable selected
  std::vector<std::string> handleLeftVarSelectedRightBasic(
      std::string& basic_value) override {
    // Uses(s, "x")
    return pkb->getLineUsesVar(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightVarSelectedLeftBasic(
      std::string& basic_value) override {
    // Uses(3, v)
    return pkb->getVarUsedByLine(basic_value)
        .value_or(std::vector<std::string>());
  }
  bool handleLeftVarSelectedRightUnderscore(std::string& arg_value) override {
    // Uses(s, _)
    auto res = pkb->getVarUsedByLine(arg_value).has_value();
    return res ? true : false;
  }
  bool handleRightVarSelectedLeftUnderscore(std::string&) override {
    std::cout << "Should not happen: ModifiesS first arg cannot be _\n";
    assert(false);
  }
  bool handleLeftVarSelectedRightVarUnselected(
      std::string& arg_select, std::string& arg_unselect) override {
    // Uses(s, v)
    return pkb->isLineUsesVar(arg_select, arg_unselect) ? true : false;
  }

  bool handleRightVarSelectedLeftVarUnselected(
      std::string& arg_unselect, std::string& arg_select) override {
    // Uses(s1, s)
    return pkb->isLineUsesVar(arg_unselect, arg_select) ? true : false;
  }

  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineUsesVarSetEmpty();
  }
  bool handleBothVarsUnselected(std::string& left_arg,
                                std::string& right_arg) override {
    // Uses(s1, s2)
    return pkb->isLineUsesVar(left_arg, right_arg) ? true : false;
  }
  std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string& arg) override {
    // Uses(s1, "x")
    return handleLeftVarSelectedRightBasic(arg);
  }
  std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string& arg) override {
    // Uses(3, v1)
    return handleRightVarSelectedLeftBasic(arg);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Uses(3, _)
    return pkb->getVarUsedByLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Uses(_, "x")
    return pkb->getLineUsesVar(arg).has_value();
  }
  bool handleLeftVarUnselectedRightUnderscore(std::string& arg) override {
    // Uses(s1, _) --> is there a statement that modifies anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSelectedRightUnderscore(arg);
  }
  bool handleRightVarUnselectedLeftUnderscore(std::string& arg) override {
    // Uses(_, v1) --> is there a variable that is modified?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSelectedLeftUnderscore(arg);
  }
  bool handleDoubleBasic(std::string& arg1, std::string& arg2) override {
    // Uses(2, "v")?
    return pkb->isLineUsesVar(arg1, arg2);
  }
};