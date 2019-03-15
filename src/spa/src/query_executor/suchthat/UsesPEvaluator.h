#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class UsesPEvaluator : public SuchThatEvaluator {
 public:
  UsesPEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Uses(p, "x")
    return pkb->getProcedureUsesVar(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Uses("first", v)
    return pkb->getVarUsedByProcedure(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Uses(p, _)
    return pkb->getVarUsedByProcedure(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string&) override {
    std::cout << "Should not happen: UsesS first arg cannot be _\n";
    assert(false);
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // Uses(p, v)
    return pkb->isProcedureUsesVar(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    return !pkb->isProcedureUsesVarSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Uses("first", _)
    return pkb->getVarUsedByProcedure(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Uses(_, "x")
    return pkb->getProcedureUsesVar(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // Uses("first", "v")?
    return pkb->isProcedureUsesVar(arg_left, arg_right);
  }
};