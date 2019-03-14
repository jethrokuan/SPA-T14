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

class CallsEvaluator : public SuchThatEvaluator {
 public:
  CallsEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Calls(p, "first")
    if (auto beforeLine = pkb->getCallerProcedures(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Calls("first", p)
    if (auto afterLine = pkb->getCalleeProcedures(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Calls(p, _) (for each p)
    return pkb->getCalleeProcedures(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Calls(_, p) (for each p)
    return pkb->getCallerProcedures(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // Calls(p1, p2)
    return pkb->isProcedureCallsProcedure(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    // Calls(_, _)
    return !pkb->isProcedureCallProcedureSetEmpty();
    assert(false);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Calls("first", _)
    return pkb->getCalleeProcedures(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Calls(_, "first")
    return pkb->getCallerProcedures(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // Calls("first", "second")
    return pkb->isProcedureCallsProcedure(arg_left, arg_right);
  }
};