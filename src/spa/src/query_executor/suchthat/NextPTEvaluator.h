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

class NextPTEvaluator : public SuchThatEvaluator {
 public:
  NextPTEvaluator(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                  PKBManager* pkb, ConstraintDatabase& db)
      : SuchThatEvaluator(decls, relCond, pkb, db){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // NextP*(s, 3)
    return pkb->getPreviousLineTBip(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // NextP*(3, s)
    return pkb->getNextLineTBip(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // NextP*(s, _) (for each s)
    return pkb->getNextLineTBip(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // NextP*(_, s) (for each s)
    return pkb->getPreviousLineTBip(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // NextP*(s, s1)
    return pkb->isLineNextLineTBip(arg_left, arg_right);
  }
  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    // NextP*(_, _)
    // This is not using a potential isLineNextLineTSetEmpty
    // Cannot compute beforehand, and the info is same for both sets
    // Next(1,2) ==> NextP*(1,2)
    // NextP*(1,2) ==> some combination of Next
    // Either both are empty or neither
    return !pkb->isLineNextLineMapEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // NextP*(3, _)
    return pkb->getNextLineTBip(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // NextP*(_, 3)
    return pkb->getPreviousLineTBip(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // NextP*(2, 3)?
    return pkb->isLineNextLineTBip(arg_left, arg_right);
  }
};