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

class CallsTEvaluator : public SuchThatEvaluator {
 public:
  CallsTEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Calls*(p, "first")
    return pkb->getCallerProceduresT(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Calls*("first", p)
    return pkb->getCalleeProceduresT(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Calls*(p, _) (for each p)
    return pkb->getCalleeProceduresT(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Calls*(_, p) (for each p)
    return pkb->getCallerProceduresT(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Calls*(p1, p2)
    // return pkb->isProcedureCallsProcedureS(arg_select, arg_unselect);
    // TODO: no such procedure
    assert(false);
  }
  bool handleDoubleUnderscore() override {
    // Calls*(_, _)
    // TODO after getting PKB call for this
    // return !pkb->isLineFollowLineSSetEmpty();
    assert(false);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Calls*("first", _)
    return pkb->getCalleeProceduresT(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Calls*(_, "first")
    return pkb->getCallerProceduresT(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Calls*("first", "second")
    // return pkb->isProcedureCallsProcedureS(arg1, arg2);
    // TODO: no such procedure
    assert(false);
  }
};