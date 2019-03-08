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

  std::vector<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Uses(s, "x")
    return pkb->getLineUsesVar(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Uses(3, v)
    return pkb->getVarUsedByLine(basic_value)
        .value_or(std::vector<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Uses(s, _)
    auto res = pkb->getVarUsedByLine(arg_value).has_value();
    return res ? true : false;
  }
  bool handleRightSynonymLeftUnderscore(std::string&) override {
    std::cout << "Should not happen: ModifiesS first arg cannot be _\n";
    assert(false);
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Uses(s, v)
    return pkb->isLineUsesVar(arg_select, arg_unselect) ? true : false;
  }
  bool handleDoubleUnderscore() override {
    return !pkb->isLineUsesVarSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Uses(3, _)
    return pkb->getVarUsedByLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Uses(_, "x")
    return pkb->getLineUsesVar(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Uses(2, "v")?
    return pkb->isLineUsesVar(arg1, arg2);
  }
};