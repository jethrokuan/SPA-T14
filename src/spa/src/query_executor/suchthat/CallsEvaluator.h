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
    // Calls(s, 3)
    if (auto beforeLine = pkb->getCallerProcedures(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Calls(3, s)
    if (auto afterLine = pkb->getCalleeProcedures(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Calls(s, _) (for each s)
    return pkb->getCalleeProcedures(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Calls(_, s) (for each s)
    return pkb->getCallerProcedures(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Calls(s1, s2)
    return pkb->isProcedureCallsProcedure(arg_select, arg_unselect);
  }
  bool handleDoubleUnderscore() override {
    // Calls(_, _)
    // TODO after getting PKB call for this
    // return !pkb->isLineFollowLineSSetEmpty();
    assert(false);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Calls(3, _)
    return pkb->getCalleeProcedures(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Calls(_, 3)
    return pkb->getCallerProcedures(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Calls(2, 3)
    return pkb->isProcedureCallsProcedure(arg1, arg2);
  }
};